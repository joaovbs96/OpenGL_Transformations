
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

#include <GL/glew.h>     // interact with OpenGL
#include <GLFW/glfw3.h>  // create windows

// vertex shader
const GLchar* vertexSource = R"glsl(
  #version 400

  // Input vertex data, different for all executions of this shader.
  layout(location = 0) in vec3 position;

  // Values that stay constant for the whole mesh.
  uniform mat4 MVP;

  void main() { 
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = MVP * vec4(position, 1.0); 
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
  int width = 600, height = 600;
  GLFWwindow* window;  // created window

  // initiate glfw
  if (glfwInit() == 0) {
    std::cerr << "GLFW failed to initiate." << std::endl;
    return -1;
  }

  // check if window was created
  window = glfwCreateWindow(width, height, "A window", nullptr, nullptr);
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

  // view matrix parameters
  glm::vec3 camPos(0.0f, 0.f, -3.0f);
  glm::vec3 camTarget(0.0f, 0.0f, 0.0f);
  glm::vec3 up(0.0f, 1.0f, 0.0f);

  // perspective matrix parameters
  float aspect = float(width) / float(height);
  float fov = glm::radians(45.0f);
  float near = 0.1f;
  float far = 100.f;

  // get MVP matrix handle
  GLuint matrixId = glGetUniformLocation(shaderProgram, "MVP");

  bool moveCamera = true;

  while (glfwWindowShouldClose(window) == 0) {
    // user input

    // check if camera can be moved
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
      moveCamera = !moveCamera;
    }

    // camera movement
    if (moveCamera) {
      const float cameraSpeed = 0.05f;
      const glm::vec3 camFront(0.0f, 0.0f, -1.0f);

      if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
        camTarget += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
      }
      if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camPos -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
        camTarget -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
      }
      if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camPos += cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
        camTarget += cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
      }
      if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camPos -= cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
        camTarget -= cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
      }
    }

    glm::mat4 model = glm::mat4(1.0f);  // identity
    glm::mat4 view = glm::lookAt(camPos, camTarget, up);
    glm::mat4 perspective = glm::perspective(fov, aspect, near, far);
    glm::mat4 mvp = perspective * view * model;  // P * V * M
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

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
