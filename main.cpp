#include <ruler_battle.h>

GLFWwindow* window;
Camera camera;

int main() {

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
  window = glfwCreateWindow(1024, 768, "Ruler Battle", NULL, NULL);
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
  glfwPollEvents();
  glfwSetCursorPos(window, 1024/2, 768/2); // set mouse cursor to the center of the window

  // set background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  // enable depth test and cull triangles whose normal is back to camera
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_CULL_FACE);

  // load shader from path
  GLuint program_id = loadShaders("shader/shader.vert", "shader/shader.frag");

  // initialize vertex data
  // create VAO
  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  // create VBO
  // static const GLfloat g_vertex_buffer_data[] = {
    // 2.0f,  0.0f, -1.0f,
   // -2.0f,  0.0f, -1.0f,
   // -2.0f,  0.0f,  1.0f,
   // -2.0f,  0.0f,  1.0f,
    // 2.0f,  0.0f,  1.0f,
    // 2.0f,  0.0f, -1.0f
  // };

  VecArray vertex_buffer_data, uv_vertex_buffer_data, normal_vertex_buffer_data;
  bool result = load_obj("model/ruler.obj", vertex_buffer_data, uv_vertex_buffer_data, normal_vertex_buffer_data);
  if(!result) {
    return 0;
  }
  vertex_buffer_data.push_back(glm::vec3( 2.0f, 0.0f, -1.0f));
  vertex_buffer_data.push_back(glm::vec3(-2.0f, 0.0f, -1.0f));
  vertex_buffer_data.push_back(glm::vec3(-2.0f, 0.0f,  1.0f));
  vertex_buffer_data.push_back(glm::vec3(-2.0f, 0.0f,  1.0f));
  vertex_buffer_data.push_back(glm::vec3( 2.0f, 0.0f,  1.0f));
  vertex_buffer_data.push_back(glm::vec3( 2.0f, 0.0f, -1.0f));

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(glm::vec3), &vertex_buffer_data[0], GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // initialize MVP matrix
  GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
  // GLuint model_matrix_id = glGetUniformLocation(program_id, "M");
  // GLuint view_matrix_id = glGetUniformLocation(program_id, "V");

  glm::mat4 desk_model_matrix = glm::mat4(1.0f);
  glm::mat4 ruler_model_matrix = glm::scale(desk_model_matrix, glm::vec3(0.06f, 0.06f, 0.06f));
  // ruler_model_matrix = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) * ruler_model_matrix;
  // set camera postion
  // glm::mat4 view_matrix = camera.getViewMatrix();
  // glm::mat4 projection_matrix = camera.getProjectionMatrix();
  glm::mat4 vp_matrix = camera.getProjectionMatrix() * camera.getViewMatrix();
  glm::mat4 ruler_mvp_matrix = vp_matrix * ruler_model_matrix;
  glm::mat4 desk_mvp_matrix = vp_matrix * desk_model_matrix;

  // animation loop until Esc is pressed
  do{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getDeviceInput();
    vp_matrix = camera.getProjectionMatrix() * camera.getViewMatrix();
    ruler_mvp_matrix = vp_matrix * ruler_model_matrix;
    desk_mvp_matrix = vp_matrix * desk_model_matrix;
    
    glUseProgram(program_id);

    // write matrix into program
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &ruler_mvp_matrix[0][0]);
    // printf("%s\n", glm::to_string(mvp_matrix));

    // write vertex attribute data to gpu
    // vertex postion data, postion = 0
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // draw triangles
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size() - 6);

    // glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &desk_mvp_matrix[0][0]);
    // glDrawArrays(GL_TRIANGLES, vertex_buffer_data.size() - 6, 6);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS 
       && glfwWindowShouldClose(window) == 0);

  glfwTerminate();
  return 0;
}
