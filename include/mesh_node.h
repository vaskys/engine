#ifndef MESH_NODE_H
#define MESH_NODE_H

#include "Node.h"
#include "model_node.h"

class MeshNode: public Node
{
    public:
        MeshNode(string name,ModelNode *model_node);
        virtual ~MeshNode(); 
        void copy(Node *target,ModelNode *m_node);
       
        vector<int> mesh_indices;

        void update();
        ModelNode *model_node;
};



#endif
