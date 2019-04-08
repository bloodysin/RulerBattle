#include <ruler_battle.h>

#include <glm/ext.hpp>
#include <iostream>

GLFWwindow* window;
Camera camera;
float delta_time;
glm::vec3 cursor_click_point;

int main() {
  cursor_click_point = glm::vec3(10.0f, 10.0f, 10.0f);
  glm::vec3 light_pos(-3.0f, 4.0f, -4.0f);

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
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwPollEvents();
  glfwSetCursorPos(window, 960/2, 720/2); // set mouse cursor to the center of the window

  // set background
  glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
  // enable depth test and cull triangles whose normal is back to camera
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_CULL_FACE);

  
  // create objects and bind the pointer to window pointer
  std::vector<Ruler> ruler_list;
  ruler_list.push_back(Ruler(0, glm::vec3( 0.0f, 0.0f, 0.0f), 0.5f * 3.14f));
  ruler_list.push_back(Ruler(1, glm::vec3( 1.0f, 0.0f, 0.0f), 0.5f * 3.14f));
  glfwSetWindowUserPointer(window, &ruler_list);

  // load model information
  std::vector<glm::vec3> vertex_buffer_data, normal_vertex_buffer_data;
  std::vector<glm::vec2> uv_vertex_buffer_data;
  bool result = initialize_ruler(vertex_buffer_data, uv_vertex_buffer_data, normal_vertex_buffer_data);
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

  normal_vertex_buffer_data.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  normal_vertex_buffer_data.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  normal_vertex_buffer_data.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  normal_vertex_buffer_data.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  normal_vertex_buffer_data.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  normal_vertex_buffer_data.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

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
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(glm::vec3),
      &vertex_buffer_data[0], GL_STATIC_DRAW);
  
  // create uv buffer
  GLuint uv_vertex_buffer;
  glGenBuffers(1, &uv_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, uv_vertex_buffer_data.size() * sizeof(glm::vec2),
      &uv_vertex_buffer_data[0], GL_STATIC_DRAW);

  // create normal buffer
  GLuint normal_vertex_buffer;
  glGenBuffers(1, &normal_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, normal_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, normal_vertex_buffer_data.size() * sizeof(glm::vec3),
      &normal_vertex_buffer_data[0], GL_STATIC_DRAW);
  
  // load texture
  GLuint ruler_tex_id = load_texture_png("model/ruler.png");
  GLuint desk_tex_id = load_texture_png("model/desk.png");
  GLuint sampler_id = glGetUniformLocation(program_id, "textureSampler");

  // initialize MVP matrix
  GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
  GLuint view_matrix_id = glGetUniformLocation(program_id, "V");
  GLuint model_matrix_id = glGetUniformLocation(program_id, "M");

  glm::mat4 desk_model_matrix = glm::mat4(1.0f);
  // set camera postion
  glm::mat4 view_matrix = camera.getViewMatrix();
  glm::mat4 vp_matrix = camera.getProjectionMatrix() * view_matrix;
  glm::mat4 ruler1_model_matrix = ruler_list[0].getModelMatrix();
  glm::mat4 ruler2_model_matrix = ruler_list[1].getModelMatrix();
  glm::mat4 ruler_mvp_matrix = vp_matrix * ruler1_model_matrix;
  glm::mat4 ruler_mvp_matrix2 = vp_matrix * ruler2_model_matrix;
  glm::mat4 desk_mvp_matrix = vp_matrix * desk_model_matrix;

  GLuint light_id = glGetUniformLocation(program_id, "light_position_world");
  GLuint material_id = glGetUniformLocation(program_id, "material_ambient");

  double last_time = glfwGetTime();
  double current_time = glfwGetTime();

  // animation loop until Esc is pressed
  do{
    current_time = glfwGetTime();
    delta_time = float(current_time - last_time);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getDeviceInput();
    ruler_list[0].update();
    ruler_list[1].update();

    view_matrix = camera.getViewMatrix();
    vp_matrix = camera.getProjectionMatrix() * view_matrix;
    ruler1_model_matrix = ruler_list[0].getModelMatrix();
    ruler2_model_matrix = ruler_list[1].getModelMatrix();
    ruler_mvp_matrix = vp_matrix * ruler1_model_matrix;
    ruler_mvp_matrix2 = vp_matrix * ruler2_model_matrix;
    desk_mvp_matrix = vp_matrix * desk_model_matrix;
    
    glUseProgram(program_id);
    
    // write matrix into program
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &ruler_mvp_matrix[0][0]);
    glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &view_matrix[0][0]);
    glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &ruler1_model_matrix[0][0]);

    // write light information to GPU
    glUniform3f(light_id, light_pos.x, light_pos.y, light_pos.z);
    // write texture material information to GPU
    glUniform3f(material_id, 0.58f, 0.58f, 0.58f);

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

    // normal vertex position data, position = 2
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vertex_buffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // draw rulers
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size() - 6);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &ruler_mvp_matrix2[0][0]);
    glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &ruler2_model_matrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size() - 6);

    // draw desk
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &desk_mvp_matrix[0][0]);
    glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &desk_model_matrix[0][0]);
    glUniform3f(material_id, 0.8f, 0.8f, 0.8f);
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
  glDeleteBuffers(1, &normal_vertex_buffer);
  glDeleteProgram(program_id);
  glDeleteTextures(1, &ruler_tex_id);
  glDeleteTextures(1, &desk_tex_id);
  glDeleteVertexArrays(1, &vertex_array_id);
  // free(&ruler_list);

  glfwTerminate();
  return 0;
}
