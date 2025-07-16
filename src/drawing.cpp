#define MOCHA_DRAWING

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace
{
mocha::Model cube;

void initCube()
{
  float vertices[] = {
     1, -1, -1,
     1, -1,  1,
    -1, -1,  1,
    -1, -1, -1,
     1,  1, -1, 
     1,  1,  1,
    -1,  1,  1,
    -1,  1, -1
  };
  unsigned int indices[] = {
    5, 1, 4,
    5, 4, 8,
    3, 7, 8,
    3, 8, 4,
    2, 6, 3,
    6, 7, 3,
    1, 5, 2,
    5, 6, 2,
    5, 8, 6,
    8, 7, 6,
    1, 2, 3,
    1, 3, 4
  };

  unsigned int vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

  glBindVertexArray(0);

  cube.vao = vao;
  cube.indices_count = (int)(sizeof(indices)/sizeof(*indices));
}
}

namespace mocha 
{
void clearColor(Color color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}

void drawModel(Model m)
{
  glBindVertexArray(m.vao);
  glDrawElements(GL_TRIANGLES, m.indices_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void drawModel(Model m, glm::mat4 trans)
{
  shaderSet(core.render.current_shader, "uTrans", trans);
  drawModel(m);
}

void drawCube(glm::vec3 pos, glm::vec3 size, Color color)
{
  // cube not initialized
  if (cube.indices_count == 0)
  {
    initCube();
  }
  glBindVertexArray(cube.vao);
  glDrawElements(GL_TRIANGLES, cube.indices_count, GL_UNSIGNED_INT, 0);
}

void shaderSet(Shader shader, const std::string& name, bool b)
{
  glUniform1i(glGetUniformLocation(shader.id, name.c_str()), (int)b);
}

void shaderSet(Shader shader, const std::string& name, int i)
{
  glUniform1i(glGetUniformLocation(shader.id, name.c_str()), i);
}

void shaderSet(Shader shader, const std::string& name, float f)
{
  glUniform1f(glGetUniformLocation(shader.id, name.c_str()), f);
}

void shaderSet(Shader shader, const std::string& name, glm::vec2 v2)
{
  glUniform2f(glGetUniformLocation(shader.id, name.c_str()), v2.x, v2.y);
}

void shaderSet(Shader shader, const std::string& name, glm::vec3 v3)
{
  glUniform3f(glGetUniformLocation(shader.id, name.c_str()), v3.x, v3.y, v3.z);
}

void shaderSet(Shader shader, const std::string &name, Color c)
{
  glUniform4f(glGetUniformLocation(shader.id, name.c_str()), c.r, c.g, c.b, c.a);
}

void shaderSet(Shader shader, const std::string &name, glm::mat4 m)
{
  glUniformMatrix4fv(glGetUniformLocation(shader.id, name.c_str()), 1, GL_FALSE, &m[0][0]);
}

void shaderUse(Shader shader)
{
  glUseProgram(shader.id);
}

}