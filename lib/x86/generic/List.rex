import IO;
import Parser;
import Pointer;
import Memory;

namespace con {
    struct Node[
        pub void* data,
        pub u32 size,
        pub void* next
    ];

    struct List[
        Node* first,
        u32 size,
        mem::Allocator* alloc
    ];

    impl List{
        pub void init(List* self,mem::Allocator* alloc){
            self->alloc = alloc;
            self->size = 0;
            self->first = null;
        }
        pub void clear(List* self){
            con::Node* n = self->first;
            
            for u32 i = 0,i<self->size,i+=1 {
                self->alloc->free(n->data);
                
                con::Node* swap = n;
                n = n->next;
                self->alloc->free(swap);
            }
            
            self->first = null;
            self->size = 0;
        }
        pub void delete(List* self){
            self->clear();
            self->alloc = null;
        }
        pub List new(mem::Allocator* alloc){
            List list;
            list.init(alloc);
            return list;
        }

        pub Node* find(List* self,u32 index){
            if index < 0 || index >= self->size {
                return null;
            }
            
            con::Node* element = self->first;
            for i32 i = index,i > 0,i -= 1 {
                element = element->next;
            }
            return element;
        }

        pub void* get(List* self,u32 i){
            if i<0 || i>=self->size || self->size<=0 {
                return null;
            }
            con::Node* element = self->find(i);
            return element->data;
        }
        pub void set(List* self,u32 i,void* element,u32 elementsize){
            if i>=0 && i<self->size {
                con::Node* n = self->find(i);
                self->alloc->free(n->data);
                
                n->size = elementsize;
                n->data = self->alloc->alloc(elementsize);
                ptr::cpy(n->data,(void*)element,elementsize);
            }
        }

        pub u32 size(List* self){
            return self->size;
        }
        
        pub void push(List* self,void* element,u32 elementsize){
            con::Node* n = self->alloc->alloc(24);
            n->size = elementsize;
            n->data = self->alloc->alloc(elementsize);
            ptr::cpy(n->data,(void*)element,elementsize);
            
            n->next = self->first;
            self->first = n;
            self->size+=1;
        }
        pub void pop(List* self){
            con::Node* first = self->first;
            if self->first != null {
                con::Node* next = first->next;
                self->alloc->free(first->data);
                self->alloc->free(first);
                self->first = next;
                self->size-=1;
            }
        }

        pub void print(List* self){
            io::print("--------------------- List -------------------\n");
            
            i8[70] cstr;
            ptr::set(cstr,32,69);
            ptr::cpy(cstr+68,"\n",2);
            ptr::cpy(cstr,"| Header:  Count: ",17);
            parse::uint::get(cstr+20,(u32)self->size);
            io::print(cstr);

            for u32 i = 0,i<self->size,i+=1 {
                con::Node* element = self->find(i);
                u32 visual = 0;
                
                if element->size==1 {
                    visual = *((u8*)element->data);
                }elif element->size==2 {
                    visual = *((u16*)element->data);
                }elif element->size==4 {
                    visual = *((u32*)element->data);
                }elif element->size==8 {
                    visual = *((u32*)element->data);
                }
                
                ptr::set(cstr,32,69);
                ptr::cpy(cstr,"| Element: ",11);
                
                parse::uint::get(cstr+14,(u32)i);

                ptr::cpy(cstr+18,"Size: ",6);
                parse::uint::get(cstr+24,element->size);
                parse::uint::get(cstr+38,visual);

                ptr::cpy(cstr+68,"\n",2);
                io::print(cstr);
            }
            io::print("----------------------------------------------\n");
        }
    }
}



