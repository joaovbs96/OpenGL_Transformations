
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

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

void translation(glm::mat4& model, const glm::vec3& vec) {
  model = glm::translate(model, vec);
}

void rotationX(glm::mat4& model, const float& angle) {
  float radians = glm::radians(angle);
  model = glm::rotate(model, radians, glm::vec3(1.0f, 0.0f, 0.0f));
}

void rotationY(glm::mat4& model, const float& angle) {
  float radians = glm::radians(angle);
  model = glm::rotate(model, radians, glm::vec3(0.0f, 1.0f, 0.0f));
}

void rotationZ(glm::mat4& model, const float& angle) {
  float radians = glm::radians(angle);
  model = glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f));
}

void scaleX(glm::mat4& model, const float& factor) {
  model = glm::scale(model, glm::vec3(factor, 1.0f, 1.0f));
}

void scaleY(glm::mat4& model, const float& factor) {
  model = glm::scale(model, glm::vec3(1.0f, factor, 1.0f));
}

void scaleZ(glm::mat4& model, const float& factor) {
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, factor));
}

#define NUM_POLYGONS 2

// global variables for key callbacks
// view matrix parameters
glm::vec3 camPos(0.0f, 0.f, -3.0f);
glm::vec3 camTarget(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::mat4 models[NUM_POLYGONS];
int selected = -1;
bool moveCamera = true;

void key_callback(GLFWwindow* window, int key, int code, int action, int mode) {
  // apply changes to all shapes or just one
  if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
    selected = -1;
  }
  if ((key >= GLFW_KEY_1 && key <= GLFW_KEY_9) && action == GLFW_PRESS) {
    selected = key - 49;
    char buffer[50];
    sprintf_s(buffer, sizeof(buffer), "selected shape: %d", selected);
    glfwSetWindowTitle(window, buffer);
  }

  // check if we should translate shapes or move camera
  if (key == GLFW_KEY_C && action == GLFW_PRESS) {
    moveCamera = !moveCamera;
  }

  // camera movement
  const float moveSpeed = 0.05f;
  if (moveCamera) {
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
      camPos += moveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
      camTarget += moveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
      camPos -= moveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
      camTarget -= moveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
      camPos += moveSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
      camTarget += moveSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
      camPos -= moveSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
      camTarget -= moveSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
      camPos += moveSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
      camTarget += moveSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
    }
    if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
      camPos -= moveSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
      camTarget -= moveSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
    }
  }

  // translation
  else {
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
      if (selected == -1)
        for (int i = 0; i < NUM_POLYGONS; i++)
          translation(models[i], glm::vec3(0.0f, 0.05f, 0.0f));
      else
        translation(models[selected], glm::vec3(0.0f, 0.05f, 0.0f));
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
      if (selected == -1)
        for (int i = 0; i < NUM_POLYGONS; i++)
          translation(models[i], glm::vec3(0.0f, -0.05f, 0.0f));
      else
        translation(models[selected], glm::vec3(0.0f, -0.05f, 0.0f));
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
      if (selected == -1)
        for (int i = 0; i < NUM_POLYGONS; i++)
          translation(models[i], glm::vec3(0.05f, 0.0f, 0.0f));
      else
        translation(models[selected], glm::vec3(0.05f, 0.0f, 0.0f));
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
      if (selected == -1)
        for (int i = 0; i < NUM_POLYGONS; i++)
          translation(models[i], glm::vec3(-0.05f, 0.0f, 0.0f));
      else
        translation(models[selected], glm::vec3(-0.05f, 0.0f, 0.0f));
    }
    if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
      if (selected == -1)
        for (int i = 0; i < NUM_POLYGONS; i++)
          translation(models[i], glm::vec3(0.0f, 0.0f, 0.05f));
      else
        translation(models[selected], glm::vec3(0.0f, 0.0f, 0.05f));
    }
    if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
      if (selected == -1)
        for (int i = 0; i < NUM_POLYGONS; i++)
          translation(models[i], glm::vec3(0.0f, 0.0f, -0.05f));
      else
        translation(models[selected], glm::vec3(0.0f, 0.0f, -0.05f));
    }
  }

  // rotation
  if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    if (selected == -1)
      for (int i = 0; i < NUM_POLYGONS; i++) rotationX(models[i], -30.0f);
    else
      rotationX(models[selected], -30.0f);
  }
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    if (selected == -1)
      for (int i = 0; i < NUM_POLYGONS; i++) rotationY(models[i], -30.0f);
    else
      rotationY(models[selected], -30.0f);
  }
  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    if (selected == -1)
      for (int i = 0; i < NUM_POLYGONS; i++) rotationZ(models[i], -30.0f);
    else
      rotationZ(models[selected], -30.0f);
  }

  // scale
  if (key == GLFW_KEY_X && action == GLFW_PRESS) {
    float factor = 0.9f;
    if (mode == GLFW_MOD_SHIFT) factor = 1.1f;

    if (selected == -1)
      for (int i = 0; i < NUM_POLYGONS; i++) scaleX(models[i], factor);
    else
      scaleX(models[selected], factor);
  }
  if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
    float factor = 0.9f;
    if (mode == GLFW_MOD_SHIFT) factor = 1.1f;

    if (selected == -1)
      for (int i = 0; i < NUM_POLYGONS; i++) scaleY(models[i], factor);
    else
      scaleY(models[selected], factor);
  }
  if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
    float factor = 0.9f;
    if (mode == GLFW_MOD_SHIFT) factor = 1.1f;

    if (selected == -1)
      for (int i = 0; i < NUM_POLYGONS; i++) scaleZ(models[i], factor);
    else
      scaleZ(models[selected], factor);
  }

  // debug
  if (key == GLFW_KEY_J && action == GLFW_PRESS) {
    std::cout << "selected shape:" << selected << std::endl;
    for (int i = 0; i < NUM_POLYGONS; i++)
      std::cout << glm::to_string(models[i]) << std::endl;
  }
}

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
      0.0f,  0.0f,  0.0f,  // vertex 3
      -1.5f, -1.5f, 0.0f,  // vertex 1
      -1.0f, -1.5f, 0.0f,  // vertex 2
      -1.0f, -1.0f, 0.0f   // vertex 3
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

  // perspective matrix parameters
  float aspect = float(width) / float(height);
  float fov = glm::radians(45.0f);
  float near = 0.1f;
  float far = 100.f;

  // get matrix handles
  GLuint mvpId = glGetUniformLocation(shaderProgram, "MVP");

  // one model matrix per shape
  for (int i = 0; i < NUM_POLYGONS; i++) models[i] = glm::mat4(1.0f);

  glfwSetKeyCallback(window, key_callback);

  while (glfwWindowShouldClose(window) == 0) {
    // process user input
    glfwPollEvents();

    // clear the window
    glClearColor(0.0f, 0.0f, 76.f / 255.f, 1.0f);  // backgroung color
    glClear(GL_COLOR_BUFFER_BIT);

    // Push matrix, set the values, draw polygons, pop, repeat for all groups
    for (int i = 0; i < NUM_POLYGONS; i++) {
      glm::mat4 view = glm::lookAt(camPos, camTarget, up);
      glm::mat4 perspective = glm::perspective(fov, aspect, near, far);
      glm::mat4 mvp = perspective * view * models[i];  // P * V * M
      glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);

      // TODO: adapt to multiple shapes
      glDrawArrays(GL_TRIANGLES, i * 3, (i * 3) + 3);
    }

    // swap buffer
    glfwSwapBuffers(window);
  }

  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glfwTerminate();

  return 0;
}
