//
// Created by Maxim Morgunov on 15.05.2019.
//

#ifndef SUFFERING2_WALLMODEL_H
#define SUFFERING2_WALLMODEL_H

float vertices[] = {
        // positions           // texture coords
        0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // top left
};

#endif //SUFFERING2_WALLMODEL_H
