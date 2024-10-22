//
// Created by Zeben on 10/22/2024.
//

#ifndef EWRENDER_CAMERA_H
#define EWRENDER_CAMERA_H

#include "..\ew\external\glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FIELD_OF_VIEW = 60.0f;

class Camera{

public:

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float fieldOfView;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float Yaw = YAW, float Pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fieldOfView(FIELD_OF_VIEW)
    {
        Position = position;
        WorldUp = up;
        yaw = Yaw;
        pitch = Pitch;
        updateCameraVectors();
    }

    Camera(float positionX, float positionY, float positionZ, float upX, float upY, float upZ, float Yaw, float Pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fieldOfView(FIELD_OF_VIEW)
    {
        Position = glm::vec3(positionX, positionY, positionZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        yaw = Yaw;
        pitch = Pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yOffset);

private:
    void updateCameraVectors();

};


#endif //EWRENDER_CAMERA_H
