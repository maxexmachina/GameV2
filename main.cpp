#include "libs.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned scrWidth = 800;
const unsigned scrHeight = 600;

float vertices[] = {
        //Position                          //Texcoord
        0.5f, 0.5f, 0.0f,                   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,                  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,                 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,                  0.0f, 1.0f
};

float mixerValue = 0.5;
float scaleAm = 1.0f;

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(scrWidth, scrHeight, "Jeff", nullptr, nullptr);
    if (!window) {
        std::cout << "Window creation failure" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    Shader myShader("/Users/savage/Documents/Suffering2/vertexShader.glsl", "/Users/savage/Documents/Suffering2/fragmentShader.glsl");
/*
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    unsigned indices[] = {
            3, 1, 0,
            3, 2, 1
    };

    unsigned VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    GLuint Texture1;
    glGenTextures(1, &Texture1);
    glBindTexture(GL_TEXTURE_2D, Texture1);
    //Texture wrapping/filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Load and generate texture
    int width, height, clChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("/Users/savage/Documents/stb_image/brick.jpeg", &width, &height, &clChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);

    GLuint Texture2;
    glGenTextures(1, &Texture2);
    glBindTexture(GL_TEXTURE_2D, Texture2);
    //Texture wrapping/filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Load and generate texture
    data = stbi_load("/Users/savage/Documents/stb_image/shrek2.png", &width, &height, &clChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);


    myShader.use();

    myShader.setInt("Texture1", 0);
    myShader.setInt("Texture2", 1);


    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


       /* float timeValue = glfwGetTime();
        float redValue = (sin(timeValue)/2.0f) + 0.5f;
        float greenValue = (cos(timeValue)/2.0f) + 0.5f;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        myShader.set4Floats("ourColor", redValue, greenValue, 0.0f, 1.0f);
        glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Texture2);

        myShader.setFloat("mixer", mixerValue);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        GLint modLoc = glGetUniformLocation(myShader.ID, "model");
        glUniformMatrix4fv(modLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);



        glfwSwapBuffers(window);
        glFlush();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "ESC Pressed";
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixerValue += 0.01f;
        if (mixerValue >= 1.0f)
            mixerValue = 1.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixerValue -= 0.01f;
        if (mixerValue <= 0.0f)
            mixerValue = 0.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        scaleAm -= 0.01f;
        if (scaleAm <= 0.0f)
            scaleAm = 0.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        scaleAm += 0.01f;
        if (scaleAm >= 1.0f)
            scaleAm = 1.0f;
    }

}
