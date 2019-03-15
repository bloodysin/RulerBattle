#include <ruler_battle.h>

#include <glm/ext.hpp>
#include <iostream>

GLFWwindow* window;
Camera camera;
float delta_time;
glm::vec3 cursor_click_point;

int main() {
  cursor_click_point = glm::vec3(10.0f, 10.0f, 10.0f);

  if(!glfwInit()) {
    fprintf(stderr, "Fail to initialize GLFW.");
    getchar();
    return -1;
  }

  // glfw initial window settings
  glfwWindowHint(GLFW_SAMPLES, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open the window ( 1024x768 )
  window = glfwCreateWindow(960, 720, "Ruler Battle", NULL, NULL);
  if(window == NULL) {
    fprintf(stderr, "Fail to open window.");
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // initialize glew
  if(glewInit() != GLEW_OK) {
    fprintf(stderr, "Fail to initialize glew.");
    getchar();
    glfwTerminate();
    return -1;
  }

  // Remain the key input until glfwGetKey is called
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  // glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwPollEvents();
  glfwSetCursorPos(window, 960/2, 720/2); // set mouse cursor to the center of the window

  // set background
  glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
  // enable depth test and cull triangles whose normal is back to camera
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_CULL_FACE);

  
  // create objects
  Ruler ruler1(glm::vec3(0.0f, 0.0f, 0.0f));
  Ruler ruler2(glm::vec3(0.0f, 0.0f, 1.0f));
  // load model information
  std::vector<glm::vec3> vertex_buffer_data, normal_vertex_buffer_data;
  std::vector<glm::vec2> uv_vertex_buffer_data;
  // bool result = load_obj("model/ruler.obj", vertex_buffer_data, uv_vertex_buffer_data, normal_vertex_buffer_data);
  bool result = ruler1.initialize(vertex_buffer_data, uv_vertex_buffer_data, normal_vertex_buffer_data);
  if(!result) {
    return 0;
  }
  // hard coding model information of the desk
  vertex_buffer_data.push_back(glm::vec3( 2.0f, 0.0f, -1.5f));
  vertex_buffer_data.push_back(glm::vec3(-2.0f, 0.0f, -1.5f));
  vertex_buffer_data.push_back(glm::vec3(-2.0f, 0.0f,  1.5f));
  vertex_buffer_data.push_back(glm::vec3(-2.0f, 0.0f,  1.5f));
  vertex_buffer_data.push_back(glm::vec3( 2.0f, 0.0f,  1.5f));
  vertex_buffer_data.push_back(glm::vec3( 2.0f, 0.0f, -1.5f));
  uv_vertex_buffer_data.push_back(glm::vec2(1.0f, 1.0f));
  uv_vertex_buffer_data.push_back(glm::vec2(0.0f, 1.0f));
  uv_vertex_buffer_data.push_back(glm::vec2(0.0f, 0.0f));
  uv_vertex_buffer_data.push_back(glm::vec2(0.0f, 0.0f));
  uv_vertex_buffer_data.push_back(glm::vec2(1.0f, 0.0f));
  uv_vertex_buffer_data.push_back(glm::vec2(1.0f, 1.0f));

  // load shader from path
  GLuint program_id = loadShaders("shader/shader.vert", "shader/shader.frag");

  // initialize vertex data
  // create VAO
  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  // create VBO
  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(glm::vec3), &vertex_buffer_data[0], GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  
  // create uv buffer
  GLuint uv_vertex_buffer;
  glGenBuffers(1, &uv_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, uv_vertex_buffer_data.size() * sizeof(glm::vec2), &uv_vertex_buffer_data[0], GL_STATIC_DRAW);
  
  // load texture
  GLuint ruler_tex_id = load_texture_png("model/ruler.png");
  GLuint desk_tex_id = load_texture_png("model/desk.png");
  GLuint sampler_id = glGetUniformLocation(program_id, "textureSampler");

  // initialize MVP matrix
  GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
  // GLuint model_matrix_id = glGetUniformLocation(program_id, "M");
  // GLuint view_matrix_id = glGetUniformLocation(program_id, "V");

  glm::mat4 desk_model_matrix = glm::mat4(1.0f);
  // glm::mat4 ruler_model_matrix = glm::scale(desk_model_matrix, glm::vec3(0.06f, 0.06f, 0.06f));
  // ruler_model_matrix = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) * ruler_model_matrix;
  // set camera postion
  glm::mat4 vp_matrix = camera.getProjectionMatrix() * camera.getViewMatrix();
  glm::mat4 ruler_mvp_matrix = vp_matrix * ruler1.getModelMatrix();
  std::cout << "ruler1: " << glm::to_string(ruler1.getModelMatrix()) << std::endl;
  glm::mat4 ruler_mvp_matrix2 = vp_matrix * ruler2.getModelMatrix();
  glm::mat4 desk_mvp_matrix = vp_matrix * desk_model_matrix;

  double last_time = glfwGetTime();
  double current_time = glfwGetTime();
  // animation loop until Esc is pressed
  do{
    current_time = glfwGetTime();
    delta_time = float(current_time - last_time);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getDeviceInput();
    vp_matrix = camera.getProjectionMatrix() * camera.getViewMatrix();
    ruler_mvp_matrix = vp_matrix * ruler1.getModelMatrix();
    ruler_mvp_matrix2 = vp_matrix * ruler2.getModelMatrix();
    desk_mvp_matrix = vp_matrix * desk_model_matrix;
    
    glUseProgram(program_id);

    // write matrix into program
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &ruler_mvp_matrix[0][0]);

    // write texture into program
    // texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ruler_tex_id);
    glUniform1i(sampler_id, 0);

    // write vertex attribute data to gpu
    // vertex postion data, position = 0
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // uv vertex position data, position = 1
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vertex_buffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // draw rulers
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size() - 6);
    // glDrawArrays(GL_TRIANGLES, 0, test);
    // glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &ruler_mvp_matrix2[0][0]);
    // glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size() - 6);

    // draw desk
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &desk_mvp_matrix[0][0]);
    glBindTexture(GL_TEXTURE_2D, desk_tex_id);
    glDrawArrays(GL_TRIANGLES, vertex_buffer_data.size() - 6, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glfwSwapBuffers(window);
    glfwPollEvents();
    last_time = current_time;
  } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS 
       && glfwWindowShouldClose(window) == 0);

  // clean buffers
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteBuffers(1, &uv_vertex_buffer);
  glDeleteProgram(program_id);
  glDeleteTextures(1, &ruler_tex_id);
  glDeleteTextures(1, &desk_tex_id);
  glDeleteVertexArrays(1, &vertex_array_id);

  glfwTerminate();
  return 0;
}
