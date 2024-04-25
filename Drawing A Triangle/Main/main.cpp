#include <string.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

static const char* vShader = "                                  \n\
#version 330                                                    \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
     gl.Position = vec4(0.4*pos.x, 0.3*pos.y, pos.z, 1.0);      \n\
}";

static const char* fShader = "                                  \n\
#version 330                                                    \n\
out vec4 color;                                                 \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
     = vec4(1.0, 0.0, 0.0, 1.0);                                \n\
}";


void CreateTriangle() 
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    
    GLint result = 0;
    
    GLchar eLog[1024]  = {0};
    
    glLinkProgram(shader);
    glGetProgramiv(theShader, GL_COMPILE_STATUS, &result);
    
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }
    
    glAttachShader(theProgram, theShader);
}

void CompileSheders() {
    shader = glCreateProgram();
    
    if (!shader) {
        printf("Error Creating Shader Program!\n");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    
    GLchar eLog[1024]  = {0};
    
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }
    
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
    
    glValidateProgram(shader);
}

int main() {
    
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialise GLFW\n");
        return -1;
    }
    
    // Setup GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Create a windowed mode window and its OpenGL context
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent(mainWindow);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }
    
    // Get framebuffer size
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    printf("%d, %d", bufferWidth, bufferHeight);
    
    // Set viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    CreateTriangle();
    CompileSheders();
    
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(mainWindow)) {
        // Poll for and process events
        glfwPollEvents();
        
        // Clear the color buffer
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Black background
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader);
        
        // Swap front and back buffers
        glfwSwapBuffers(mainWindow);
    }
    
    // Clean up
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    
    return 0;
}
