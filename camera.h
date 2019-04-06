//
// Created by Maxim Morgunov on 30.03.2019.
//

#ifndef SUFFERING2_CAMERA_H
#define SUFFERING2_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include <vector>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

//Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.2f;

class Camera {
public:
    //Camera attibutes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 displacement;
    //Euler angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;

    //Constructor
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }
    void processKeyboard(CameraMovement direction, float deltaTime) {
        glm::vec3 dx, dz;
        float velocity = MovementSpeed * 0.01f;
        if (direction == FORWARD) {
            glm::vec3 p1z = Position;
            Position += Front * velocity;
            dz = Position - p1z;
        }
        if (direction == BACKWARD) {
            glm::vec3 p1z = Position;
            Position -= Front * velocity;
            dz = Position - p1z;
        }
        if (direction == RIGHT) {
            glm::vec3 p1x = Position;
            Position += Right * velocity;
            dx = Position - p1x;
        }
        if (direction == LEFT) {
            glm::vec3 p1x = Position;
            Position -= Right * velocity;
            dx = Position - p1x;
        }
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
        Position.y = 0.0f;
        displacement = dx + dz;

    }

    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        Front = glm::normalize(front);
        //Recalculate Right and Up vectors
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};



#endif //SUFFERING2_CAMERA_H
