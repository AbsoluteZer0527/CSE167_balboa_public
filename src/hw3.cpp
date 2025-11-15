#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"

using namespace hw3;


void hw_3_1(const std::vector<std::string> &params) {
    // HW 3.1: Open a window using GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);    

      while (!glfwWindowShouldClose(window))
    {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // Rendering commands
        glClearColor(0.6745f, 0.6118f, 1.0f, 1.0f);  // #ac9cff
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    glfwTerminate();
}

void hw_3_2(const std::vector<std::string> &params) {
    // HW 3.2: Render a single 2D triangle
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "HW 3.2 - Rotating Triangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    
    glViewport(0, 0, 800, 600);

    // Vertex shader with rotation
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform float rotation;
        
        void main() {
            // Create rotation matrix for z-axis rotation
            float cosTheta = cos(rotation);
            float sinTheta = sin(rotation);
            
            // Apply rotation
            float x = aPos.x * cosTheta - aPos.y * sinTheta;
            float y = aPos.x * sinTheta + aPos.y * cosTheta;
            
            gl_Position = vec4(x, y, aPos.z, 1.0);
        }
    )";
    // Fragment shader with constant color
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(0.6745, 0.6118, 1.0, 1.0);  // Your favorite #ac9cff color!
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders into program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f   // top center
    };

    // Create VAO and VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind VAO first
    glBindVertexArray(VAO);
    
    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // Dark background
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use shader program
        glUseProgram(shaderProgram);
        
        // Update rotation angle based on time
        float timeValue = glfwGetTime();
        float rotationAngle = timeValue;  // Rotate at 1 radian per second
        
        // Set the rotation uniform
        int rotationLoc = glGetUniformLocation(shaderProgram, "rotation");
        glUniform1f(rotationLoc, rotationAngle);
        
        // Draw triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}

void hw_3_3(const std::vector<std::string> &params) {
    // HW 3.3: Render a scene
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    int width = scene.camera.resolution.x;
    int height = scene.camera.resolution.y;
    GLFWwindow* window = glfwCreateWindow(width, height, "HW 3.3 - 3D Scene", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    
    glViewport(0, 0, width, height);
    
    // Enable depth testing (CRITICAL for 3D!)
    glEnable(GL_DEPTH_TEST);
    
    // Enable gamma correction
    glEnable(GL_FRAMEBUFFER_SRGB);

     // Vertex shader - handles 3D transformations
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        
        out vec3 vertexColor;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

     // Fragment shader - outputs interpolated colors
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(vertexColor, 1.0);
        }
    )";
    
    // Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

     // Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

     unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Create VAO and VBO for each mesh
    struct MeshBuffers {
        unsigned int VAO;
        unsigned int VBO_vertices;
        unsigned int VBO_colors;
        int vertexCount;
    };

    std::vector<MeshBuffers> meshBuffers;
    
    for (const auto& mesh : scene.meshes) {
        MeshBuffers buffers;
        
        // Convert vertex data to float arrays
        std::vector<float> vertices;
        std::vector<float> colors;
        
        for (const auto& face : mesh.faces) {
            for (int i = 0; i < 3; i++) {
                // Get vertex position
                Vector3 pos = mesh.vertices[face[i]];
                vertices.push_back(pos.x);
                vertices.push_back(pos.y);
                vertices.push_back(pos.z);
                
                // Get vertex color
                Vector3 color = mesh.vertex_colors[face[i]];
                colors.push_back(color.x);
                colors.push_back(color.y);
                colors.push_back(color.z);
            }
        }
         buffers.vertexCount = vertices.size() / 3;
        
        // Create VAO
        glGenVertexArrays(1, &buffers.VAO);
        glBindVertexArray(buffers.VAO);
        
        // Create and bind vertex position VBO
        glGenBuffers(1, &buffers.VBO_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Create and bind vertex color VBO
        glGenBuffers(1, &buffers.VBO_colors);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO_colors);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
        
        meshBuffers.push_back(buffers);
    }
     // Camera state for interaction
    Vector3 cameraPos = Vector3{scene.camera.cam_to_world(0, 3), 
                                 scene.camera.cam_to_world(1, 3), 
                                 scene.camera.cam_to_world(2, 3)};
    
    // Extract camera directions from cam_to_world matrix
    Vector3 cameraRight = Vector3{scene.camera.cam_to_world(0, 0),
                                   scene.camera.cam_to_world(1, 0),
                                   scene.camera.cam_to_world(2, 0)};
    
    Vector3 cameraUp = Vector3{scene.camera.cam_to_world(0, 1),
                                scene.camera.cam_to_world(1, 1),
                                scene.camera.cam_to_world(2, 1)};
    
    Vector3 cameraForward = Vector3{-scene.camera.cam_to_world(0, 2),
                                     -scene.camera.cam_to_world(1, 2),
                                     -scene.camera.cam_to_world(2, 2)};
    
    float cameraSpeed = 0.05f;
    float lastFrame = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Input - WASD camera movement
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        float velocity = cameraSpeed * deltaTime * 60.0f; // Normalize for frame rate
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos = cameraPos + cameraForward * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos = cameraPos - cameraForward * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos = cameraPos - cameraRight * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos = cameraPos + cameraRight * velocity;
        
        // Clear buffers (BOTH color and depth!)
        glClearColor(scene.background.x, scene.background.y, scene.background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use shader program
        glUseProgram(shaderProgram);
        
        // Create view matrix (camera transformation)
        Matrix4x4f view = Matrix4x4f::identity();
        
        // Build view matrix from camera position and orientation
        Vector3 target = cameraPos + cameraForward;
        
        // Simplified view matrix using lookAt concept
        view(0, 0) = cameraRight.x;
        view(0, 1) = cameraRight.y;
        view(0, 2) = cameraRight.z;
        view(0, 3) = -dot(cameraRight, cameraPos);
        
        view(1, 0) = cameraUp.x;
        view(1, 1) = cameraUp.y;
        view(1, 2) = cameraUp.z;
        view(1, 3) = -dot(cameraUp, cameraPos);
        
        view(2, 0) = -cameraForward.x;
        view(2, 1) = -cameraForward.y;
        view(2, 2) = -cameraForward.z;
        view(2, 3) = dot(cameraForward, cameraPos);
        
        view(3, 0) = 0;
        view(3, 1) = 0;
        view(3, 2) = 0;
        view(3, 3) = 1;
        
        // Create projection matrix (perspective)
        float aspect = (float)width / (float)height;
        float s = scene.camera.s;
        float z_near = scene.camera.z_near;
        float z_far = scene.camera.z_far;
        
        Matrix4x4f projection = Matrix4x4f::identity();
        projection(0, 0) = 1.0f / (aspect * s);
        projection(1, 1) = 1.0f / s;
        projection(2, 2) = -z_far / (z_far - z_near);
        projection(2, 3) = -(z_far * z_near) / (z_far - z_near);
        projection(3, 2) = -1.0f;
        projection(3, 3) = 0.0f;
        
        // Send view and projection matrices to shader
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view(0, 0));
        
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection(0, 0));
        
        // Render each mesh
        for (size_t i = 0; i < scene.meshes.size(); i++) {
            // Get model matrix for this mesh
            Matrix4x4f model = scene.meshes[i].model_matrix;
            
            // Send model matrix to shader
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model(0, 0));
            
            // Draw the mesh
            glBindVertexArray(meshBuffers[i].VAO);
            glDrawArrays(GL_TRIANGLES, 0, meshBuffers[i].vertexCount);
        }
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    for (auto& buffers : meshBuffers) {
        glDeleteVertexArrays(1, &buffers.VAO);
        glDeleteBuffers(1, &buffers.VBO_vertices);
        glDeleteBuffers(1, &buffers.VBO_colors);
    }
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}

void hw_3_4(const std::vector<std::string> &params) {
    // HW 3.4: Render a scene with lighting
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
}
