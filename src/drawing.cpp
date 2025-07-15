#define MOCHA_DRAWING

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

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

void shaderSet(Shader shader, const std::string& name, Vector2 v2)
{
  glUniform2f(glGetUniformLocation(shader.id, name.c_str()), v2.x, v2.y);
}

void shaderSet(Shader shader, const std::string& name, Vector3 v3)
{
  glUniform3f(glGetUniformLocation(shader.id, name.c_str()), v3.x, v3.y, v3.z);
}

void shaderUse(Shader shader)
{
  glUseProgram(shader.id);
}

}