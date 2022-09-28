#ifndef NODE_H
#define NODE_H

#include "config.h"
#include "set"
#include "transform.h"
#include "controler.h"


class Node
{
    public:
        Node(string name);
        virtual ~Node();
        
        virtual void copy(Node *target);
        string get_name();
        Node *get_parent();
        Node *get_child(string n);

        vector<Node*> get_children();
        Transform get_transform();
    
        void set_name(string v);
        void set_parent(Node *node);
        void add_child(Node *node);
        virtual void set_transform(Transform transform_i);

        void delete_nodes(Node *node);

        void delete_child_nodes();
        void delete_child_node(string n);

        void unparent(Node *target);
        void unparent_child_nodes(Node *target);
        void unparent_child_node(string n,Node *target);

        virtual void update();
        void update_t_children();
        bool can_modify;
        bool inherit_t;

        void set_controler(Controler *con);
        Controler *get_controler();
    private:
        Node *parent;
        vector<Node*> children;
        string name;

        Transform transform;

        string check_for_node_names(string v);

        Controler *controler;
};


#endif
