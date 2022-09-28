#include "Node.h"
#include "engine.h"
#include "lights.h"

//////////////////////////////////
//
Node::Node(string name)
{
    this->name=name;
    children.clear();
    parent=nullptr;

    can_modify=true;
    inherit_t=true;

    controler=NULL;
}

Node::~Node()
{
   // delete_nodes(this);
   //
   if(controler != nullptr)
       delete controler;
}

void Node::copy(Node *target)
{
    Node *nova=new Node(get_name());
    nova->set_transform(get_transform());
    target->add_child(nova);

    for(int i=0;i<get_children().size();i++)
    {
        Node *decko=get_children().at(i);
        decko->copy(nova);
    }
}

string Node::check_for_node_names(string v)
{
    
    string node_name=v;
    short it=0;
    bool c=false;
    do
    {
        c=false;
        for(int i=0;i<get_children().size();i++)
        {
            if(get_children().at(i)->get_name() == node_name)
            {
                c=true;
                node_name=v+to_string(it);
                it++;
            }
        }
    }while(c);

    return node_name;

}
string Node::get_name()
{
    return name;
}

Node *Node::get_parent()
{
    return parent;
}

Node *Node::get_child(string n)
{
    for(int i=0;i<children.size();i++)
    {
        if(children.at(i)->get_name() == n )
            return children.at(i);
    }
    return nullptr;
}

vector<Node*> Node::get_children()
{
    return children;
}

Transform Node::get_transform()
{
    return transform;
}

void Node::set_parent(Node*node)
{
    parent=node;
}

void Node::set_name(string v)
{
    string name_check=v;
    if(get_parent() != nullptr)
        name_check=get_parent()->check_for_node_names(v);
    this->name=name_check;
    for(int i=0;i<children.size();i++)
    {
        string n=check_for_node_names(children.at(i)->get_name());
        children.at(i)->set_name(n);
    }
}

void Node::add_child(Node *node)
{
    node->set_parent(this);
    node->set_name(node->get_name());
    children.push_back(node);
}

void Node::set_transform(Transform transform_i)
{
    transform=transform_i;
    transform.dirty=true;
}


void Node::delete_nodes(Node *node)
{
    for(int i=0;i<node->get_children().size();i++)
    {
        delete_nodes(node->get_children().at(i));
    }
    
    debug_print("deleting node "+node->get_name());
    delete node;
}


void Node::delete_child_nodes()
{
    for(int i=0;i<children.size();i++)
    {
        delete_nodes(children.at(i));           
    }
    children.clear();
}

void Node::delete_child_node(string n)
{
    Node *node_to_remove=get_child(n);
    node_to_remove->set_parent(nullptr);
    children.erase(remove(children.begin(),children.end(),node_to_remove),children.end());
    delete_nodes(node_to_remove);
}

void Node::unparent_child_nodes(Node *target)
{
  //  if(can_modify)
    {
       for(int i=0;i<children.size();i++)
       {
           children.at(i)->unparent(target);
       }
    }
}

void Node::unparent_child_node(string n,Node *target)
{
//    if(can_modify)
    {
        Node *node_to_remove=get_child(n);
        children.erase(remove(children.begin(),children.end(),node_to_remove),children.end());
        if(target != NULL)
            target->add_child(node_to_remove);
    }
}

void Node::unparent(Node *target)
{
    if(can_modify)
    {
        Node *active_parent=get_parent();
        if(active_parent != nullptr)
        {
            active_parent->unparent_child_node(get_name(),target);
        }
    }
}


void Node::update_t_children()
{
    {
        if(get_parent() == NULL)
        {
            transform.set_global_transform(transform.get_local_tranform());
        }
        else if(!inherit_t)
            transform.set_global_transform(transform.get_local_tranform());
        else
        {
            transform.set_global_transform(get_parent()->transform.get_global_transform()*transform.get_local_tranform()); 
        }


    }
}


//////////////////////////////////////////////


void Node::update()
{
   // if(transform.dirty)
    {
        update_t_children();
        transform.dirty=false;
    }
    if(controler)
        controler->update();
}



void Node::set_controler(Controler *con_i)
{
    controler=con_i;
}

Controler *Node::get_controler()
{
    return controler;
}




