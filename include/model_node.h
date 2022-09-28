#ifndef MODEL_NODE_H
#define MODEL_NODE_H

#include "model.h"
#include "Node.h"
#include "config.h"
#include "animator.h"
#include "animation.h"

class ModelNode : public Node
{
    public:
        ModelNode(string name,Model *model);
        virtual ~ModelNode();

        Model *model;
        void update();
        void copy(Node *target);

        bool has_bones();
        Animator *get_animator();
    private:
        Animator *animator;

};

#endif
