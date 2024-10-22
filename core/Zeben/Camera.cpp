//
// Created by Zeben on 10/22/2024.
//

#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
    {
        Position += Front * velocity;
    }
    if (direction == BACKWARD)
    {
        Position -= Front * velocity;
    }
    if (direction == LEFT)
    {
        Position -= Right * velocity;
    }
    if (direction == RIGHT)
    {
        Position += Right * velocity;
    }
    if (direction == SPRINT)
    {
        Position += Front * (velocity * 2);
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    float Yaw;
    float Pitch;
    Yaw = yaw;
    Pitch = pitch;
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;



    yaw += xOffset;
    pitch += yOffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f) {Pitch = 89.0f;}
        if (Pitch < -89.0f) {Pitch = -89.0f;}
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
    fieldOfView -= (float)yOffset;
    if (fieldOfView < 1.0f) {fieldOfView = 1.0f;}
    if (fieldOfView > 120.0f) {fieldOfView = 120.0f;}
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}