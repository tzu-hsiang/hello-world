'''
演算法分析基測
學號: 10441319 / 10626305 / 10627255
姓名: 楊宗玲   / 魏子翔   / 蔡怡萱
系級: 財金系   / 資工系   / 資工系
'''

class Node:
    
    _upper = 4 # node can contain item number 
    
    def __init__(self):
    
        self._num_of_Items = 0
        self._parent = None
        self._item_array = []    # items of this node 
        self._child_array = []   # childs of this node
        
        for i in range(self._upper - 1):
            self._item_array.append(None)
            
        for i in range(self._upper):    
            self._child_array.append(None)

    # connect new node to this node as a child
    def connect_child(self, childNum, new_child):
    
        self._child_array[childNum] = new_child
        if new_child != None:
            new_child._parent = self    

    # disconnect child from this node, return it (node)
    def disconnect_child(self, childNum):
    
        cut_node = self._child_array[childNum]
        self._child_array[childNum] = None
        
        return cut_node

    def get_child(self, childNum):
        return self._child_array[childNum]

    def get_parent(self):
        return self._parent

    def is_Leaf(self):
        return self._child_array[0] == None

    def get_items_num(self):
        return self._num_of_Items

    def get_item(self, index): 
        return self._item_array[index]

    def is_Full(self):
        return self._num_of_Items == self._upper - 1

    def insert_item(self, new_item): # return index of new inserted item
    
        self._num_of_Items += 1
        new_key = new_item
        
        for i in reversed(range(self._upper - 1)): # start on right
        
            if self._item_array[i] != None:
                cur_key = self._item_array[i]
                
                if new_key < cur_key: 
                    self._item_array[i + 1] = self._item_array[i]   # shift origin right
                else:
                    self._item_array[i + 1] = new_item 
                    return i + 1
            
        self._item_array[0] = new_item 
        return 0

    def cut_item(self): # cut a largest item of node, and return it
    
        cut_item = self._item_array[self._num_of_Items - 1]
        self._item_array[self._num_of_Items - 1] = None
        self._num_of_Items -= 1
        
        return cut_item

    def print_node(self):
    
        print('(', end = '')
        length = len(range(self._num_of_Items))
        
        for i in range(self._num_of_Items):
            item = self._item_array[i]
            print(item, end = '')
            
            if i != length - 1:
                print(', ', end = '')
        
        print(')', end = '')

# end class Node

class Tree234:

    def __init__(self): # create a node
        self.root = Node()

    def insert(self, new_value): # insert a node
        cur_node = self.root

        while True:
            if cur_node.is_Full(): # need to split node
                self.split(cur_node)    
                cur_node = cur_node.get_parent() 
                cur_node = self.get_insert_item_child(cur_node, new_value)

            elif cur_node.is_Leaf(): 
                break   
            
            else:
                cur_node = self.get_insert_item_child(cur_node, new_value)
        
        cur_node.insert_item(new_value)
    
    def get_insert_item_child(self, node, new_value): # get correct node to insert new item
        numItems = node.get_items_num()
        
        i = 0 
        for i in range(numItems): 
            if new_value < node.get_item(i): 
                return node.get_child(i)
                
        return node.get_child(i + 1)

    def split(self, cur_node): # if node is full, split the node
       
        new_right_item = cur_node.cut_item() # it will insert to new right node
        new_root_item = cur_node.cut_item() # it will insert to parent or new root node
        new_right_child_0 = cur_node.disconnect_child(2)  
        new_right_child_1 = cur_node.disconnect_child(3)  

        new_right = Node()

        if cur_node == self.root:    
            self.root = Node()   
            parent = self.root 
            parent.connect_child(0, cur_node)
            
        else: # current node is not the root
            parent = cur_node.get_parent()

        # new parent
        item_inserted_index = parent.insert_item(new_root_item)
        parent_items_num = parent.get_items_num()

        idx = parent_items_num - 1
        
        # shift origin connection to right, then insert new connection to correct position
        while idx > item_inserted_index:    
            node = parent.disconnect_child(idx)  
            parent.connect_child(idx + 1, node)    
            idx -= 1
        
        parent.connect_child(item_inserted_index + 1, new_right)

        # new right
        new_right.insert_item(new_right_item)
        
        # insert origin right part connection to new right node
        new_right.connect_child(0, new_right_child_0)
        new_right.connect_child(1, new_right_child_1)

    def __preorder(self, root):
        root.print_node() 
        numItems = root.get_items_num()
        
        for i in range(numItems + 1):
            cur_node = root.get_child(i)
            if cur_node:
                self.__preorder(cur_node)
            else:
                return

    def __postorder(self, root):
        numItems = root.get_items_num()
        
        for i in range(numItems + 1):
            cur_node = root.get_child(i)
            if cur_node:
                self.__preorder(cur_node)
            else:
                return
        
        root.print_node() #display this node
                
    def print_preorder(self):
        self.__preorder(self.root)
        
    def print_postorder(self):
        self.__postorder(self.root)

# end class Tree234


if __name__ == '__main__':
    tree = Tree234()
    arr = list(map(int, input("Enter a multiple value: ").split()))
    
    for i in arr:
        tree.insert(i)
    
    print('\npreorder:')
    tree.print_preorder()
    print('\npostorder:')
    tree.print_postorder()
    
    del tree
