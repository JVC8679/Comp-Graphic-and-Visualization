//============================================================================
// Name        : CS-330_3D Scene
// Author      : James Carver
// Version     : 7-1 Final
// Copyright   : 
// Description : Added final touches to lighting and textures 
//============================================================================


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "cylinder.h"
#include "ShapeGenerator.h"
#include "ShapeData.h"
#include "Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"



using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 3.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// sphere
GLuint sphereNumIndices;
GLuint sphereVertexArrayObjectID;
GLuint sphereIndexByteOffset;

// Position , Normals , Texture
float boxVertices[] = {
    // Vertex Positions         //normals         //Texture
    // depth  height  width        
    //left
     0.5f,  -0.0f,  2.0f,  -1.0f,  0.0f, 0.0f,  1.0f, 0.0f, //  Top left              
    -0.0f,  -0.0f,  2.0f,  -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, //  Bottom left           
    -0.0f,   2.5f,  2.0f,  -1.0f,  0.0f, 0.0f,  0.0f, 1.0f, //  Top left              
    -0.0f,   2.5f,  2.0f,  -1.0f,  0.0f, 0.0f,  0.0f, 1.0f, //  Top left              
     0.5f,   2.5f,  2.0f,  -1.0f,  0.0f, 0.0f,  1.0f, 1.0f, //  Top right          
     0.5f,  -0.0f,  2.0f,  -1.0f,  0.0f, 0.0f,  1.0f, 0.0f, //  Bottom right         
     //front
     0.5f,   2.5f,  2.0f,   0.0f,  0.0f, 1.0f,  0.0f, 1.0f, //  Top right             
     0.5f,  -0.0f, -0.0f,   0.0f,  0.0f, 1.0f,  1.0f, 0.0f, //  Bottom left           
     0.5f,   2.5f, -0.0f,   0.0f,  0.0f, 1.0f,  1.0f, 1.0f, //  Top left           
     0.5f,  -0.0f, -0.0f,   0.0f,  0.0f, 1.0f,  1.0f, 0.0f, //  Bottom left          
     0.5f,  -0.0f,  2.0f,   0.0f,  0.0f, 1.0f,  0.0f, 0.0f, //  Bottom right        
     0.5f,   2.5f,  2.0f,   0.0f,  0.0f, 1.0f,  0.0f, 1.0f, //  Top right           
     //right
    -0.0f,   2.5f, -0.0f,   1.0f,  0.0f, 0.0f,  1.0f, 1.0f, //  Top left             
     0.5f,   2.5f, -0.0f,   1.0f,  0.0f, 0.0f,  0.0f, 1.0f, //  Top right            
     0.5f,  -0.0f, -0.0f,   1.0f,  0.0f, 0.0f,  0.0f, 0.0f, //  Bottom right         
     0.5f,  -0.0f, -0.0f,   1.0f,  0.0f, 0.0f,  0.0f, 0.0f, //  Bottom right         
    -0.0f,  -0.0f, -0.0f,   1.0f,  0.0f, 0.0f,  1.0f, 0.0f, //  Bottom left           
    -0.0f,   2.5f, -0.0f,   1.0f,  0.0f, 0.0f,  1.0f, 1.0f, //  Top left              

    -0.0f,  -0.0f, -0.0f,   0.0f, -1.5f, 0.0f,  0.0f, 0.0f, //  Bottom left         
     0.5f,  -0.0f, -0.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f, //  Bottom right          
     0.5f,  -0.0f,  2.0f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f, //  Top right             
    -0.0f,  -0.0f,  2.0f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f, //  Bottom right          
    -0.0f,  -0.0f, -0.0f,   0.0f, -1.5f, 0.0f,  0.0f, 0.0f, //  Bottom left          
     0.5f,  -0.0f,  2.0f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f, //  Top right           
     //top
    -0.0f,   2.5f,  2.0f,   0.0f,  1.0f, 0.0f,  1.0f, 1.0f, //  Top right           
     0.5f,   2.5f,  2.0f,   0.0f,  1.0f, 0.0f,  0.0f, 1.0f, //  Top left            
     0.5f,   2.5f, -0.0f,   0.0f,  1.0f, 0.0f,  0.0f, 0.0f, //  Bottom left         
    -0.0f,   2.5f,  2.0f,   0.0f,  1.0f, 0.0f,  1.0f, 1.0f, //  Top right            
    -0.0f,   2.5f, -0.0f,   0.0f,  1.0f, 0.0f,  1.0f, 0.0f, //  bottom right         
     0.5f,   2.5f, -0.0f,   0.0f,  1.0f, 0.0f,  0.0f, 0.0f, //  Bottom left          

};

float boxVertices2[] = {
    //back

    -0.0f,   2.5f,  2.0f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f, //  Top right            
    -0.0f,  -0.0f, -0.0f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f, //  Bottom left          
    -0.0f,   2.5f, -0.0f,   0.0f,  0.0f, -1.0f,  0.0f, 1.0f, //  Top left             
    -0.0f,  -0.0f,  2.0f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f, //  Bottom right        
    -0.0f,   2.5f,  2.0f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f, //  Top right           
    -0.0f,  -0.0f, -0.0f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f, //  Bottom left          
};

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-3.0f,  4.0f,   10.0f),
    glm::vec3( 6.0f,  4.0f,   -3.0f)
};

float cubeVertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

// Position , Normals , Texture
float clockVertices[] = {
    // Vertex Positions         //normals         //Texture
    // depth  height  width        
    //left
     0.3f,  -0.0f,  1.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //  Top left              
    -0.0f,  -0.0f,  1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //  Bottom left           
    -0.0f,   1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //  Top left              
    -0.0f,   1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //  Top left              
     0.3f,   1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //  Top right          
     0.3f,  -0.0f,  1.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //  Bottom right         
     //front
     0.3f,   1.0f,  1.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, //  Top right             
     0.3f,  -0.0f, -0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //  Bottom left           
     0.3f,   1.0f, -0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //  Top left           
     0.3f,  -0.0f, -0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //  Bottom left          
     0.3f,  -0.0f,  1.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //  Bottom right        
     0.3f,   1.0f,  1.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, //  Top right           
     //right
    -0.0f,   1.0f, -0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //  Top left             
     0.3f,   1.0f, -0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //  Top right            
     0.3f,  -0.0f, -0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //  Bottom right         
     0.3f,  -0.0f, -0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //  Bottom right         
    -0.0f,  -0.0f, -0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //  Bottom left           
    -0.0f,   1.0f, -0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //  Top left              

    -0.0f,  -0.0f, -0.0f,   0.0f,-1.5f, 0.0f,  0.0f, 0.0f, //  Bottom left         
     0.3f,  -0.0f, -0.0f,   0.0f,-1.0f, 0.0f,  1.0f, 0.0f, //  Bottom right          
     0.3f,  -0.0f,  1.0f,   0.0f,-1.0f, 0.0f,  1.0f, 1.0f, //  Top right             
    -0.0f,  -0.0f,  1.0f,   0.0f,-1.0f, 0.0f,  0.0f, 1.0f, //  Bottom right          
    -0.0f,  -0.0f, -0.0f,   0.0f,-1.5f, 0.0f,  0.0f, 0.0f, //  Bottom left          
     0.3f,  -0.0f,  1.0f,   0.0f,-1.0f, 0.0f,  1.0f, 1.0f, //  Top right           
     //top
    -0.0f,   1.0f,  1.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //  Top right           
     0.3f,   1.0f,  1.0f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //  Top left            
     0.3f,   1.0f, -0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //  Bottom left         
    -0.0f,   1.0f,  1.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //  Top right            
    -0.0f,   1.0f, -0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //  bottom right         
     0.3f,   1.0f, -0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //  Bottom left          
     //back
    -0.0f,   1.0f,  1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f, //  Top right            
    -0.0f,  -0.0f, -0.0f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f, //  Bottom left          
    -0.0f,   1.0f, -0.0f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f, //  Top left             
    -0.0f,  -0.0f,  1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f, //  Bottom right        
    -0.0f,   1.0f,  1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f, //  Top right           
    -0.0f,  -0.0f, -0.0f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f, //  Bottom left          
};

bool perspective = true;
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

int main() {

    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW we are using the CORE profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CS-330 3D Scene", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // set callbacks
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // load shaders
    Shader lightingShader("shaderfiles/lights.vs", "shaderfiles/lights.fs");
    Shader lightCubeShader("shaderfiles/cube.vs", "shaderfiles/cube.fs");

    // box front
    int stride = sizeof(float) * 8; // Strides between vertex coordinates is

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(VAO);
    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU
    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // box back
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices2), boxVertices2, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU
    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);


    unsigned int  VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    //clock frame
    unsigned int  VAO4;
    glGenVertexArrays(1, &VAO4); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(VAO4);
    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(clockVertices), clockVertices, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU
    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // creates sphere object
    ShapeData sphere = ShapeGenerator::makeSphere();

    // speaker
    unsigned int sphereVBO, sphereVAO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.vertexBufferSize() + sphere.indexBufferSize(), 0, GL_STATIC_DRAW);
    GLsizeiptr currentOffset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
    currentOffset += sphere.vertexBufferSize();
    sphereIndexByteOffset = currentOffset;
    glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
    sphereNumIndices = sphere.numIndices;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 6));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereVBO);

    //light cubes
    // first, configure the cube's VAO (and VBO)
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // load textures
    unsigned int front      = loadTexture("textures/front.jpg");
    unsigned int back       = loadTexture("textures/back.jpg");
    unsigned int base       = loadTexture("textures/metal.png");
    unsigned int koozie     = loadTexture("textures/koozie.png");
    unsigned int speaker    = loadTexture("textures/speaker1.png");
    unsigned int table      = loadTexture("textures/wood.png");
    unsigned int clock      = loadTexture("textures/clock.png");
    unsigned int clockFrame = loadTexture("textures/clockFrame1.jpg");

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
       // -------------------------------------------------------------------------------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);






    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // activate shader
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

 
        /*
          set all the uniforms 
        */
        // directional light
        lightingShader.setVec3("dirLight.direction", -3.0f, 2.0f, -1.5f);
        lightingShader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 0.7f, 0.7f, 0.7f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.19f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.132f);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 2.5f, 2.5f, 2.5f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 0.7f, 0.7f, 0.7f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
        // spotlight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 1.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        
        // view/projection transformations
        
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        //box front
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, front);
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.5f, 0.1f, 0.0f));
        float angle = 45.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -10.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 30);

        // box back
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, back);
        glBindVertexArray(VAO2);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.5f, 0.1f, 0.0f));
        angle = 45.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -10.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //clock frame
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, clockFrame);
        glBindVertexArray(VAO4);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(.5f, 0.1f, 0.0f));
        angle = 90.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.10f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // clock face
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, clock);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.60f, -0.03f));
        angle = -90.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(-0.01f, 0.00f, 0.0f));
        lightingShader.setMat4("model", model);
        static_meshes_3D::Cylinder C3(0.35, 40, .1, true, true, true);
        C3.render();

        // draw speaker
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, speaker);
        glBindVertexArray(sphereVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.5f, 1.35f, 0.5f));
        model = glm::scale(model, glm::vec3(.9f)); // Make it a smaller sphere
        lightingShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);
       
        // speaker base
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, base);
        glBindVertexArray(VAO3);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.5f, 0.4f, 0.5f));
        lightingShader.setMat4("model", model);
        static_meshes_3D::Cylinder C(0.8, 40, .65, true, true, true);
        C.render();

        // koozie
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, koozie);
        glBindVertexArray(VAO3);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, .98f, 0.0f));
        lightingShader.setMat4("model", model);
        static_meshes_3D::Cylinder C1(0.5, 40, 1.75, true, true, true);
        C1.render();



        // table plane
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, table);
        glBindVertexArray(VAO3);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.05f, -1.0f));
        lightingShader.setMat4("model", model);
        static_meshes_3D::Cylinder C2(6, 40, .1, true, true, true);
        C2.render();





        //// also draw the lamp object(s)
        //lightCubeShader.use();
        //lightCubeShader.setMat4("projection", projection);
        //lightCubeShader.setMat4("view", view);

        //// we now draw as many light bulbs as we have point lights.
        //glBindVertexArray(lightCubeVAO);
        //for (unsigned int i = 0; i < 2; i++)
        //{

        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, pointLightPositions[i]);
        //    model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller cube
        //    lightCubeShader.setMat4("model", model);
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteVertexArrays(1, &VAO4);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &sphereVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(LIFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(LOWER, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera.ProcessKeyboard(WIREON, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(WIREOFF, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ResetCamera();

    // change view between perspective and orthographics
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (perspective) {
            projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
            perspective = false;
        }
        else {
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            perspective = true;
        }
    }

}



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format{};
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);
}