#include "libs.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
bool ifIntersect(glm::vec3 wallPos, float rotation, glm::vec3 Pos, glm::vec3 Displacement) {

    glm::mat2 Kram1 = glm::mat2x2(wallPos.x - Pos.x, 1 * cos(rotation), wallPos.z - Pos.z, 1 * sin(rotation));
    glm::mat2 Kram2 = glm::mat2x2(Displacement.x, wallPos.x - Pos.x, Displacement.z, wallPos.z - Pos.z);
    glm::mat2 Kram = glm::mat2x2(Displacement.x, 1 * cos(rotation), Displacement.z, 1 * sin(rotation));

    float t1 = glm::determinant(Kram1) / glm::determinant(Kram);
    float t2 = glm::determinant(Kram2) / glm::determinant(Kram);

    if (t1 < 1.5 and t1 > -1.5 and t2 < -0.5 and t2 > -1.5)
        return true;
}
const unsigned scrWidth = 800;
const unsigned scrHeight = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 1.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  0.0f,  0.0f),
        glm::vec3( 3.0f,  0.0f,  0.0f),
        glm::vec3( 4.0f, 0.0f, 0.0f),
        glm::vec3( 0.0f,  0.0f, 1.0f),
        glm::vec3( 1.0f,  0.0f,  1.0f),
        glm::vec3( 2.0f,  0.0f,  1.0f),
        glm::vec3( 3.0f,  0.0f, 1.0f),
        glm::vec3( 4.0f,  0.0f, 1.0f),
        glm::vec3( -1.0f,  0.0f,  0.0f),
        glm::vec3( -2.0f,  0.0f,  0.0f),

};

float wallRotation[] {
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f
};

float vertices[] = {
        // positions           // texture coords
        0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // top left
};

/*unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
};*/

float mixerValue = 0.5;
float scaleAm = 1.0f;
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = scrWidth/2, lastY = scrHeight/2;
float yaw = 0;
float pitch = 0;
bool firstEncounter = true;

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
    glfwSetCursorPosCallback(window, mouseCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader myShader("/Users/savage/Documents/Suffering2/vertexShader.glsl", "/Users/savage/Documents/Suffering2/fragmentShader.glsl");

    glEnable(GL_DEPTH_TEST);
/*
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
    unsigned char* data = stbi_load("/Users/savage/Documents/stb_image/limestone.jpg", &width, &height, &clChannels, 0);
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
    data = stbi_load("/Users/savage/Documents/stb_image/jebaited.png", &width, &height, &clChannels, 0);
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

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(scaleAm, scaleAm, scaleAm));
        myShader.setMat4("transform", transform);

        glm::mat4 view = camera.getViewMatrix();
        myShader.setMat4("view", view);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)(scrWidth)/(float)(scrHeight), 0.001f, 100.0f);
        myShader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        for (int i = 0; i < 12; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            float angle = wallRotation[i];
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
            myShader.setMat4("model", model);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 12; i++) {
            if(ifIntersect(cubePositions[i], wallRotation[i], camera.Position, camera.displacement))
                camera.Position -= camera.displacement;
            //std::cout << "t1 = " << t1 << " ";
            //std::cout << "t2 = " << t2 << " ";
            //glm::vec2 intersect = (glm::vec2(cubePositions[1].x + 0.5*cos(wallRotation[1])*t1, cubePositions[1].z - 0.5*sin(wallRotation[1])*t1));
            //std::cout << "x = " << intersect.x << " z = " << intersect.y ;
            //std::cout << " dx = " << camera.displacement.x << " dz = " << camera.displacement.z << std::endl;
        }

        //glDrawArrays(GL_TRIANGLES, 0, 36);



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

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if(firstEncounter) {
        lastX = xpos;
        lastY = ypos;
        firstEncounter = false;
    }


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);

}

void processInput(GLFWwindow* window) {
    float cameraSpeed = 2.5f * deltaTime;

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

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.processKeyboard(DOWN, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKeyboard(UP, deltaTime);
    }



}
