
#include <iostream>
#include "glm/glm.hpp"

// MO814 - Topics in Computer Graphics
// João Vítor Buscatto Silva - RA 155951
// Allan Rossi Gomes - RA XXXXXX

// GLM documentation
// http://www.c-jump.com/bcc/common/Talk3/Math/GLM/GLM.html

// Drawing
// Basic
// https://open.gl/drawing
// Cubes
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/

// Transformations
// https://open.gl/transformations
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

// Creating 3D Shapes:
// 1° - Create shapes with unit dimensions in the origin
// 2° - Translate them to other positions of the screen

#include <GL/glew.h>     // interact with OpenGL
#include <GLFW/glfw3.h>  // create windows

// vertex shader
const GLchar* vertexSource = R"glsl(
  #version 400

  in vec3 position; 

  void main() { 
    gl_Position = vec4(position, 1.0); 
  }
)glsl";

// fragment shader
const GLchar* fragmentSource = R"glsl(
  #version 400 
  
  out vec4 outColor;

  void main() { 
    outColor = vec4(1.0, 0.0, 0.0, 1.0); 
  }
)glsl";

int main() {
  GLFWwindow* window;  // created window

  // initiate glfw
  if (glfwInit() == 0) {
    std::cerr << "GLFW failed to initiate." << std::endl;
    return -1;
  }

  // check if window was created
  window = glfwCreateWindow(600, 600, "A window", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "GLFW failed to create window." << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);

  // initiate glew
  if (glewInit() != GLEW_OK) {
    std::cerr << "GLFW failed to create window." << std::endl;
    glfwTerminate();
    return -1;
  }

  // Create Vertex Array Object
  /* Since only calls after binding a VAO stick to it, make sure that you've
   * created and bound the VAO at the start of your program. Any vertex
   * buffers and element buffers bound before it will be ignored. */
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  static const GLfloat vertices[] = {
      -0.5f, -0.5f, 0.0f,  // vertex 1
      0.0f,  -0.5f, 0.0f,  // vertex 2
      0.0f,  0.0f,  0.0f   // vertex 3
  };

  // creates a Vertex Buffer Object, and copies data to it
  GLuint vbo;             // reference number to the vbo
  glGenBuffers(1, &vbo);  // Generate 1 buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);

  // Create and compile the fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader(fragmentShader);

  // compiling program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);  // link program
  glUseProgram(shaderProgram);   // activate program - only one at a time

  // bind output
  glBindFragDataLocation(shaderProgram, 0, "outColor");

  // link vertex data and attributes
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

  while (glfwWindowShouldClose(window) == 0) {
    // clear the window
    glClearColor(0.0f, 0.0f, 76.f / 255.f, 1.0f);  // backgroung color
    glClear(GL_COLOR_BUFFER_BIT);

    // draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // swap buffer
    glfwSwapBuffers(window);

    // process user events
    glfwPollEvents();
  }

  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glfwTerminate();

  return 0;
}
