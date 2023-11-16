#pragma once

#include "utils/glm_utils.h"

#define NUM_BONES_PER_VEREX 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ZERO_MEM(a) memset(a, 0, sizeof(a))

struct VertexBoneData
{
    unsigned int IDs[NUM_BONES_PER_VEREX];
    float Weights[NUM_BONES_PER_VEREX];

    VertexBoneData()
    {
        Reset();
    };

    void Reset()
    {
        ZERO_MEM(IDs);
        ZERO_MEM(Weights);
    }

    void AddBoneData(unsigned int BoneID, float Weight)
    {
        for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
            if (Weights[i] == 0.0) {
                IDs[i] = BoneID;
                Weights[i] = Weight;
                return;
            }
        }

        // should never get here - more bones than we have space for
        assert(0);
    }
};

struct BoneInfo
{
    glm::mat4 boneOffset;
    glm::mat4 finalTransformation;
};