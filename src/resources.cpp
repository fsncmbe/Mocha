#define MOCHA_RESOURCES

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace mocha
{
std::string loadFile(const std::string& path)
{
  std::string s = core.assets.path;
  s = s.append(path);
  std::ifstream file(s);

  if(!file.is_open())
  {
    std::string error_msg = "Failed to load: ";
    error_msg = error_msg.append(s);
    log(LogLevel::ERROR, error_msg.c_str());
  }

  std::string out((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

  file.close();
  return out;
}

Shader loadShader(const std::string& name)
{
  std::string v_path = "shaders/" + name + ".vs";
  std::string v_string = loadFile(v_path);
  const char* v_shader = v_string.c_str();

  std::string f_path = "shaders/" + name + ".fs";
  std::string f_string = loadFile(f_path);
  const char* f_shader = f_string.c_str();

  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &v_shader, NULL);
  glCompileShader(vertex);

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &f_shader, NULL);
  glCompileShader(fragment);

  int id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  log(LogLevel::DEBUG, "Shader loaded!");
  return {id};
}

Model loadModel(const std::string& name)
{

  // file handling
  std::string path = "models/" + (std::string)name + ".obj";

  // add a break at the end so the loop recognizes it 
  std::string obj_string = loadFile(path) + '\n';
  const char* obj_file = obj_string.c_str();

  // vectors for data
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  std::vector<glm::vec3>      temp_positions;
  std::vector<glm::vec3>      temp_normals;
  std::vector<glm::vec2>      temp_uvs;

  // fill vertex_indices... with data
  const char* line_start = obj_file;

  for (const char* p = obj_file; *p != '\0'; ++p)
  {
    if (*p == '\n')
    {
      std::string line(line_start, p);

      std::istringstream line_stream(line);
      std::string type;

      line_stream >> type;

      // vertex
      if (type == "v")
      {
        glm::vec3 pos;
        line_stream >> pos.x >> pos.y >> pos.z;
        temp_positions.push_back(pos);
      }
      // vertex texture
      else if (type == "vt")
      {
        glm::vec2 uv;
        line_stream >> uv.x >> uv.y;
        temp_uvs.push_back(uv);
      }
      // vertex normal
      else if (type == "vn")
      {
        glm::vec3 normal;
        line_stream >> normal.x >> normal.y >> normal.z;
        temp_normals.push_back(normal);
      }
      // material
      else if (type == "usemtl")
      {
        
      }
      // face
      else if (type == "f")
      {
        const int f_nums = 3;
        std::string strings[f_nums];
        for (int i=0; i<f_nums; i++)
        {
          line_stream >> strings[i];
          unsigned int vertex_index, uv_index, normal_index;
          sscanf(strings[i].c_str(), "%d/%d/%d", &vertex_index, &uv_index, &normal_index);

          Vertex v;

          v.position  = temp_positions[vertex_index-1];
          v.tex_coord = temp_uvs[uv_index-1];
          v.normal    = temp_normals[normal_index-1];

          auto it = std::find(vertices.begin(), vertices.end(), v);

          if (it != vertices.end())
          {
            int index = std::distance(vertices.begin(), it);
            indices.push_back(index);
          } else {
            vertices.push_back(v);
            indices.push_back(vertices.size()-1);
          }
        }
      }
      line_start = p + 1;
    }
  }

  unsigned int vao, vbo, ebo;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glBindVertexArray(0);

  Model m;
  m.vao = vao;
  m.indices_count = indices.size();
  return m;
}

}