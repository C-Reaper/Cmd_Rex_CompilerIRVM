import "../lib/IO.rex";
import "../lib/Parser.rex";
import "../lib/Pointer.rex";
import "../lib/Memory.rex";

namespace con {
    struct Vector[
        u64 elementsize,
        u64 size,
        u64 memory_size,
        void* memory,
        pub mem::Allocator* alloc
    ];

    impl Vector{
        pub void init(Vector* self,mem::Allocator* alloc,u64 elementsize,u64 size){
            self->alloc = alloc;
            self->elementsize = elementsize;
            self->size = 0;
            self->memory_size = size;
            self->memory = alloc->alloc(size * elementsize);
        }
        pub void delete(Vector* self){
            void* mem = self->memory;
            self->alloc->free(mem);
            self->alloc = null;
            self->elementsize = 0;
            self->size = 0;
            self->memory_size = 0;
            self->memory = null;
        }
        pub Vector new(mem::Allocator* alloc,u64 elementsize){
            Vector vec;
            vec.init(alloc,elementsize,5);
            return vec;
        }
        pub Vector make(mem::Allocator* alloc,u64 elementsize,u64 size){
            Vector vec;
            vec.init(alloc,elementsize,size);
            return vec;
        }

        pub void* get(Vector* self,u64 i){
            if i<0 || i>=self->size {
                return null;
            }
            return self->memory + i * self->elementsize;
        }
        pub void set(Vector* self,u64 i,void* element){
            if i>=0 && i<self->size {
                ptr::cpy(self->memory + i * self->elementsize,element,self->elementsize);
            }
        }
        pub void set_c(Vector* self,u64 i,void* elements,u64 count){
            if elements != null && i>=0 && i+count<=self->size {
                ptr::cpy(self->memory + i * self->elementsize,elements,self->elementsize * count);
            }
        }
        pub void expand(Vector* self,u64 count){
            u64 length = self->size + count;
            if length > self->memory_size {
                u64 newsize = self->memory_size;
                while length>newsize {
                    newsize *= 2;
                }

                u64 elementsize = self->elementsize;
                void* block = self->alloc->alloc(newsize * elementsize);
                ptr::cpy(block,self->memory,self->size * elementsize);
                
                self->alloc->free(self->memory);
                self->memory = block;
                self->memory_size = newsize;
            }
        }
        pub void shrink(Vector* self,u64 count){
            u64 newsize = self->memory_size / 2;
            if self->size <= newsize && newsize>5 {
                u64 elementsize = self->elementsize;
                void* block = self->alloc->alloc(newsize * elementsize);
                ptr::cpy(block,self->memory,self->size * elementsize);
                
                self->alloc->free(self->memory);
                self->memory = block;
                self->memory_size = newsize;
            }
        }

        pub void tozero(Vector* self){
            ptr::set(self->memory,0,self->size * self->elementsize);
        }
        pub void clear(Vector* self){
            u64 elementsize = self->elementsize;
            void* block = self->alloc->alloc(5 * elementsize);
            self->alloc->free(self->memory);
            self->memory = block;
            self->size = 0;
            self->memory_size = 5;
        }

        pub void* get_data(Vector* self){
            return self->memory;
        }
        pub u64 get_size(Vector* self){
            return self->size;
        }
        
        pub void push(Vector* self,void* element){
            self->expand(1);
            self->size+=1;
            self->set(self->size-1,element);
        }
        pub void pop(Vector* self){
            self->size-=1;
            self->shrink(1);
        }
        
        pub void push_c(Vector* self,void* elements,u64 count){
            self->expand(count);
            self->size+=count;
            self->set_c(self->size-count,elements,count);
        }
        pub void pop_c(Vector* self,u64 count){
            if self->size<count {
                count = self->size;
            }
            self->size-=count;
            self->shrink(count);
        }

        pub void print(Vector* self){
            io::print("------------------- Vector -------------------\n");
            i8[50] cstr;
            ptr::set(cstr,32,49);
            ptr::cpy(cstr+48,"\n",2);
            ptr::cpy(cstr,"| Header: ",10);
            parse::hex::get(cstr+12,(u64)self->memory);
            parse::uint::get(cstr+30,(u64)self->elementsize);
            parse::uint::get(cstr+40,(u64)self->size);
            io::print(cstr);

            for u64 i = 0,i<self->size,i+=1 {
                void* element = self->get(i);
                u64 visiual = 0;
                
                ptr::set(cstr,32,49);
                ptr::cpy(cstr,"| Element: ",11);
                parse::uint::get(cstr+14,(u64)i);
                
                if self->elementsize==1 {
                    visiual = *((u8*)element);
                    cstr[18] = visiual;
                }elif self->elementsize==2 {
                    visiual = (u16)(*((u16*)element));
                    parse::uint::get(cstr+18,visiual);
                }elif self->elementsize==4 {
                    visiual = (u32)(*((u32*)element));
                    parse::uint::get(cstr+18,visiual);
                }else {
                    visiual = (u64)(*((u64*)element));
                    parse::uint::get(cstr+18,visiual);
                }
                
                ptr::cpy(cstr+48,"\n",2);
                io::print(cstr);
            }
            io::print("----------------------------------------------\n");
        }
    }


    struct Node[
        pub void* data,
        pub u64 size,
        pub void* next
    ];

    struct List[
        Node* first,
        u64 size,
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
            for u64 i = 0,i<self->size,i+=1 {
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

        pub Node* find(List* self,u64 i){
            if i<0 || i>=self->size {
                return null;
            }
            con::Node* element = self->first;
            for ,i>0,i-=1 {
                element = element->next;
            }
            return element;
        }

        pub void* get(List* self,u64 i){
            if i<0 || i>=self->size || self->size<=0 {
                return null;
            }
            con::Node* element = self->find(i);
            return element->data;
        }
        pub void set(List* self,u64 i,void* element,u64 elementsize){
            if i>=0 && i<self->size {
                con::Node* n = self->find(i);
                self->alloc->free(n->data);
                
                n->size = elementsize;
                n->data = self->alloc->alloc(elementsize);
                ptr::cpy(n->data,(void*)element,elementsize);
            }
        }

        pub u64 size(List* self){
            return self->size;
        }
        
        pub void push(List* self,void* element,u64 elementsize){
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
            ptr::cpy(cstr,"| Header: ",10);
            parse::uint::get(cstr+40,(u64)self->size);
            io::print(cstr);

            for u64 i = 0,i<self->size,i+=1 {
                con::Node* element = self->find(i);
                u64 visiual = 0;
                
                if element->size==1 {
                    visiual = *((u8*)element);
                }elif element->size==2 {
                    visiual = *((u16*)element);
                }elif element->size==4 {
                    visiual = *((u32*)element);
                }elif element->size==8 {
                    visiual = *((u64*)element);
                }
                
                ptr::set(cstr,32,69);
                ptr::cpy(cstr,"| Element: ",11);
                parse::uint::get(cstr+14,(u64)i);
                parse::uint::get(cstr+18,*(u64*)element);
                parse::uint::get(cstr+38,visiual);
                ptr::cpy(cstr+68,"\n",2);
                io::print(cstr);
            }
            io::print("----------------------------------------------\n");
        }
    }
}



