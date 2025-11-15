#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"
#include "3rdparty/stb_image.h"

using namespace hw3;

unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}


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

Vector3 operator*(const Vector3& v, float scalar) {
    return Vector3{v.x * scalar, v.y * scalar, v.z * scalar};
}

Vector3 operator*(float scalar, const Vector3& v) {
    return Vector3{v.x * scalar, v.y * scalar, v.z * scalar};
}

void hw_3_3(const std::vector<std::string> &params) {
    // HW 3.3: Render a scene
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    //second parameter: texture file
    std::string texture_path = "";
    bool has_scene_texture = false;
    unsigned int scene_texture = 0;
    
    if (params.size() >= 2) {
        texture_path = params[1];
        // scene_texture = loadTexture(texture_path.c_str());
        // has_scene_texture = true;
        // std::cout << "Loaded texture: " << texture_path << std::endl;
    }


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
    if (!texture_path.empty()) {
        scene_texture = loadTexture(texture_path.c_str());
        has_scene_texture = true;
        std::cout << "Loaded texture: " << texture_path << std::endl;
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
        layout (location = 2) in vec2 aTexCoord;
        
        out vec3 vertexColor;
        out vec2 TexCoord; 
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            vertexColor = aColor;
            TexCoord = aTexCoord;
        }
    )";

     // Fragment shader - outputs interpolated colors
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        in vec2 TexCoord; 
        out vec4 FragColor;
        
        uniform sampler2D texture1;
        uniform bool useTexture;

        void main() {
        if(useTexture){
            FragColor = texture(texture1, TexCoord);
        }else{
            FragColor = vec4(vertexColor, 1.0);

        }
    }
    )";
    
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
        unsigned int VBO_uvs;
        unsigned int textureID;
        int vertexCount;
        bool hasTexture;
    };

    std::vector<MeshBuffers> meshBuffers;
    
    for (const auto& mesh : scene.meshes) {
        MeshBuffers buffers;
        
        // Convert vertex data to float arrays
        std::vector<float> vertices;
        std::vector<float> colors;
        std::vector<float> uvs;
        
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

                if (!mesh.uvs.empty()) {
                Vector2 uv = mesh.uvs[face[i]];
                uvs.push_back(uv.x);
                uvs.push_back(uv.y);
                } else {
                    uvs.push_back(0.0f);
                    uvs.push_back(0.0f);
                }
            }
        }
        buffers.vertexCount = vertices.size() / 3;

        //extra: load texture
        buffers.hasTexture = has_scene_texture;
        buffers.textureID = scene_texture;
        std::string scene_path = params[0];
        std::string texture_path = "";

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

        /// Create and bind UV VBO 
        glGenBuffers(1, &buffers.VBO_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO_uvs);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

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

    // Column 2 is -forward (because of d = -d), so negate it to get forward
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
            
            if(meshBuffers[i].hasTexture){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, meshBuffers[i].textureID);
                glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
                glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
            }else{
                glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
            }
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
        glDeleteBuffers(1, &buffers.VBO_uvs);
        if (buffers.hasTexture) {
            glDeleteTextures(1, &buffers.textureID);
        }
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

    // Check for optional texture parameter
    std::string texture_path = "";
    bool has_scene_texture = false;
    unsigned int scene_texture = 0;
    
    if (params.size() >= 2) {
        texture_path = params[1];
    }

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    int width = scene.camera.resolution.x;
    int height = scene.camera.resolution.y;
    GLFWwindow* window = glfwCreateWindow(width, height, "HW 3.4 - Lighting", NULL, NULL);
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

    // Load texture AFTER OpenGL is initialized
    if (!texture_path.empty()) {
        scene_texture = loadTexture(texture_path.c_str());
        has_scene_texture = true;
        std::cout << "Loaded texture: " << texture_path << std::endl;
    }
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Vertex shader with lighting
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;
        layout (location = 3) in vec3 aNormal;
        
        out vec3 FragPos;
        out vec3 vertexColor;
        out vec2 TexCoord;
        out vec3 Normal;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            gl_Position = projection * view * vec4(FragPos, 1.0);
            vertexColor = aColor;
            TexCoord = aTexCoord;
            
            // Transform normal (use transpose of inverse for non-uniform scaling)
            Normal = mat3(transpose(inverse(model))) * aNormal;
        }
    )";

    // Fragment shader with Phong lighting
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 FragPos;
        in vec3 vertexColor;
        in vec2 TexCoord;
        in vec3 Normal;
        
        out vec4 FragColor;
        
        uniform sampler2D texture1;
        uniform bool useTexture;
        uniform vec3 lightDir;
        uniform vec3 viewPos;
        
        void main() {
            // Get object color (from texture or vertex color)
            vec3 objectColor;
            if (useTexture) {
                objectColor = texture(texture1, TexCoord).rgb;
            } else {
                objectColor = vertexColor;
            }
            
            // Lighting parameters
            float ambientStrength = 0.1;
            float specularStrength = 0.5;
            
            // Ambient
            vec3 ambient = ambientStrength * objectColor;
            
            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDirection = normalize(lightDir);
            float diff = max(dot(norm, lightDirection), 0.0);
            vec3 diffuse = diff * objectColor;
            
            // Specular
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDirection, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);
            
            // Combine
            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, 1.0);
        }
    )";

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

    // Mesh buffers structure
    struct MeshBuffers {
        unsigned int VAO;
        unsigned int VBO_vertices;
        unsigned int VBO_colors;
        unsigned int VBO_uvs;
        unsigned int VBO_normals;
        unsigned int textureID;
        int vertexCount;
        bool hasTexture;
    };

    std::vector<MeshBuffers> meshBuffers;
    
    for (const auto& mesh : scene.meshes) {
        MeshBuffers buffers;
        
        std::vector<float> vertices;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        
        for (const auto& face : mesh.faces) {
            for (int i = 0; i < 3; i++) {
                // Vertex position
                Vector3 pos = mesh.vertices[face[i]];
                vertices.push_back(pos.x);
                vertices.push_back(pos.y);
                vertices.push_back(pos.z);
                
                // Vertex color
                Vector3 color = mesh.vertex_colors[face[i]];
                colors.push_back(color.x);
                colors.push_back(color.y);
                colors.push_back(color.z);
                
                // UV coordinates
                if (!mesh.uvs.empty()) {
                    Vector2 uv = mesh.uvs[face[i]];
                    uvs.push_back(uv.x);
                    uvs.push_back(uv.y);
                } else {
                    uvs.push_back(0.0f);
                    uvs.push_back(0.0f);
                }
                
                // Vertex normals
                if (!mesh.vertex_normals.empty()) {
                    Vector3 normal = mesh.vertex_normals[face[i]];
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);
                } else {
                    normals.push_back(0.0f);
                    normals.push_back(1.0f);
                    normals.push_back(0.0f);
                }
            }
        }
        
        buffers.vertexCount = vertices.size() / 3;
        buffers.hasTexture = has_scene_texture;
        buffers.textureID = scene_texture;

        // Create VAO
        glGenVertexArrays(1, &buffers.VAO);
        glBindVertexArray(buffers.VAO);
        
        // Position VBO
        glGenBuffers(1, &buffers.VBO_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Color VBO
        glGenBuffers(1, &buffers.VBO_colors);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO_colors);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        
        // UV VBO
        glGenBuffers(1, &buffers.VBO_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO_uvs);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        
        // Normal VBO
        glGenBuffers(1, &buffers.VBO_normals);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO_normals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(3);
        
        glBindVertexArray(0);
        
        meshBuffers.push_back(buffers);
    }

    // Camera setup
    Vector3 cameraPos = Vector3{scene.camera.cam_to_world(0, 3), 
                                 scene.camera.cam_to_world(1, 3), 
                                 scene.camera.cam_to_world(2, 3)};
    
    if (cameraPos.x == 0 && cameraPos.y == 0 && cameraPos.z == 0) {
        cameraPos.z = 3.0f;
    }
    
    Vector3 cameraRight = Vector3{scene.camera.cam_to_world(0, 0),
                                   scene.camera.cam_to_world(1, 0),
                                   scene.camera.cam_to_world(2, 0)};
    
    Vector3 cameraUp = Vector3{scene.camera.cam_to_world(0, 1),
                                scene.camera.cam_to_world(1, 1),
                                scene.camera.cam_to_world(2, 1)};
    
    Vector3 cameraForward = Vector3{scene.camera.cam_to_world(0, 2),
                                     scene.camera.cam_to_world(1, 2),
                                     scene.camera.cam_to_world(2, 2)};
    
    float cameraSpeed = 5.0f;
    float lastFrame = 0.0f;

    // Light direction (normalized)
    Vector3 lightDir = Vector3{1.0f, 1.0f, 1.0f};
    float len = sqrt(lightDir.x * lightDir.x + lightDir.y * lightDir.y + lightDir.z * lightDir.z);
    lightDir.x /= len;
    lightDir.y /= len;
    lightDir.z /= len;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        float velocity = cameraSpeed * deltaTime * 60.0f;
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos = cameraPos + cameraForward * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos = cameraPos - cameraForward * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos = cameraPos - cameraRight * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos = cameraPos + cameraRight * velocity;
        
        glClearColor(scene.background.x, scene.background.y, scene.background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        // View matrix
        Matrix4x4f view = Matrix4x4f::identity();
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
        
        // Projection matrix
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
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view(0, 0));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection(0, 0));
        
        // Set lighting uniforms
        glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
        
        // Render meshes
        for (size_t i = 0; i < scene.meshes.size(); i++) {
            Matrix4x4f model = scene.meshes[i].model_matrix;
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model(0, 0));
            
            if (meshBuffers[i].hasTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, meshBuffers[i].textureID);
                glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
                glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
            } else {
                glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
            }
            
            glBindVertexArray(meshBuffers[i].VAO);
            glDrawArrays(GL_TRIANGLES, 0, meshBuffers[i].vertexCount);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    for (auto& buffers : meshBuffers) {
        glDeleteVertexArrays(1, &buffers.VAO);
        glDeleteBuffers(1, &buffers.VBO_vertices);
        glDeleteBuffers(1, &buffers.VBO_colors);
        glDeleteBuffers(1, &buffers.VBO_uvs);
        glDeleteBuffers(1, &buffers.VBO_normals);
        if (buffers.hasTexture) {
            glDeleteTextures(1, &buffers.textureID);
        }
    }
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}
