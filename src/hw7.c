#include "hw7.h"

/*  
    typedef struct {
        char name;
        unsigned int num_rows;
        unsigned int num_cols;
        int values[]; 
}   matrix_sf;

    typedef struct bst_sf {
        matrix_sf *mat;
        struct bst_sf *left_child;
        struct bst_sf *right_child;
}   bst_sf;
*/

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) { 
    bst_sf *node = root;
    if(root == NULL){ 
        // new node
        bst_sf *new_node = malloc(sizeof(bst_sf));
        new_node->mat = mat;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        return new_node;
    } 
    // Binary search tree compares sizes to determine left or right node, in general
    for(;;){
        if(mat->name < node->mat->name){ // letter names less than will be the left child of the greater node name
            if(node->left_child == NULL){
                bst_sf *new_node = malloc(sizeof(bst_sf));
                new_node->mat = mat;
                new_node->left_child = NULL;
                new_node->right_child = NULL;
                node->left_child = new_node;
                break;
            }
            else
                node = node->left_child;
        }
        else if(mat->name > node->mat->name){
            if(node->right_child == NULL){
                bst_sf *new_node = malloc(sizeof(bst_sf));
                new_node->mat = mat;
                new_node->left_child = NULL;
                new_node->right_child = NULL;
                node->right_child = new_node;
                break;
            }
            else
                node = node->right_child;
        }
        else break; // duplicate check? assumed none
    }
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) { // returns pointer to matrix
    bst_sf *node = root;
    if(root == NULL)
        return NULL;
    
    for(;;){
        if(node == NULL)
            return NULL;
        // check case for when node is not found after BST traversal, ****
        if(node->mat->name != name)
            if(node->mat->name > name)
                node = node->left_child;
            else 
                node = node->right_child;
        else if(node->mat->name == name)
            return node->mat;
        else   
            continue;
    }
    return NULL;
}

void free_bst_sf(bst_sf *root) { // TA recursive search java example
    //bst_sf *node = root;
    if(root == NULL)
        return;
    // by post order traversal, continues until hits null children, then frees backwards rec.
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);
    free(root->mat);
    free(root); 
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    if(mat1 == NULL || mat2 == NULL)
        return NULL;
    if((mat1->num_rows != mat2->num_rows) || (mat1->num_cols != mat2->num_cols))
        return NULL;
    matrix_sf *sum = malloc(sizeof(matrix_sf) + (mat1->num_rows * mat1->num_cols * sizeof(int))); // matrix sf (1+4+4+x), x = rows*cols*int_size

    sum->name = '+';
    sum->num_rows = mat1->num_rows;
    sum->num_cols = mat1->num_cols;
    int size = sum->num_rows * sum->num_cols;
    for(int i = 0; i < size; i++){
        sum->values[i] = mat1->values[i] + mat2->values[i];
    }
    return sum;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
   if(mat1 == NULL || mat2 == NULL)
        return NULL;
    if((mat1->num_cols != mat2->num_rows)) // mxn nxp, n=n
        return NULL;
    matrix_sf *product = malloc(sizeof(matrix_sf) + (mat1->num_rows * mat2->num_cols * sizeof(int))); // matrix sf (1+4+4+x), x = mat1rows * mat2cols*int_size

    product->name = '*';
    product->num_rows = mat1->num_rows;
    product->num_cols = mat2->num_cols;
    int size = product->num_rows * product->num_cols;
    for(int r = 0; r < product->num_rows; r++){ // ((row space in matrix) + the column indexes of each row)
        for(int c = 0; c < product->num_cols; c++){
        int sum_in = 0;
            for(int i = 0; i < mat1->num_cols && i < mat2->num_rows; i++){
                sum_in += (mat1->values[(r * mat1->num_cols) + i] * mat2->values[(i * mat2->num_cols) + c]); // kto1_sum(a_ri * b_ic)
            }
            product->values[(r * product->num_cols) + c] = sum_in;
        }
    }
    return product;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    if(mat == NULL)
        return NULL;
    matrix_sf *t_matrix = malloc(sizeof(matrix_sf) + (mat->num_rows * mat->num_cols * sizeof(int)));

    t_matrix->name = '^';
    t_matrix->num_rows = mat->num_cols;
    t_matrix->num_cols = mat->num_rows;
    // row column switch, fill as usual with opposite mat reference
    for(int r = 0; r < t_matrix->num_rows; r++){
        for(int c = 0; c < t_matrix->num_cols; c++){
            t_matrix->values[((r * t_matrix->num_cols) + c)] = mat->values[(c * mat->num_cols) + r];
        }
    }
    return t_matrix;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    int rows;
    int cols;
    sscanf(expr, " %d %d ", &rows, &cols);

    char *p = (char *)expr;
    while(*p != '\0'){
        if(*p == '['){
            p++; 
            break;
        }
        p++;
    }
    matrix_sf *new_matrix = malloc(sizeof(matrix_sf) + (rows * cols * sizeof(int)));
    new_matrix->name = name;
    new_matrix->num_rows = rows;
    new_matrix->num_cols = cols;
    int size = rows * cols;
    int count = 0;

    while(count < size && *p != '\0'){
        if((*p < '0' || *p > '9') && (*p != '-')){
            // skip non-nums & negative numbers
            p++;
            // pointe rleft at digit
        } 
        else{
            int num;
            sscanf(p, " %d ", &num); // pointer p is left there, -num is considered number at - by sscanf
            new_matrix->values[count] = num;
            count++; 

            if(*p == '-')
                p++;
            // move pointer to set past the number put into the initialized matrix
            while((*p >= '0') && (*p <= '9')){
                p++;
            }
        }
    }
    return new_matrix;
}

char* infix2postfix_sf(char *infix) { // stack operations as in CSE 214, approach by array
    char *postfix = malloc(strlen(infix) + 1); // +1 terminator
    char *op_stack = malloc(strlen(infix));

    int i = 0;
    int top = -1;
    int k = 0;
    // append operands directly to postfix string
    // for operators, if the operator at the stack[top] >= in precedence to current infix[i], pop it to postfix and push the current infix[i]
    // for '(' at infix[i] push onto stack, then for ')' pop everything in stack until stack[i] is '(' discard, ignore both ()
    // pop remaining operators to postfix
    while(infix[i] != '\0'){
        char p = infix[i];
        if(p >= 'A' && p <= 'Z')
            postfix[k++] = p;
        else if(p == '\'') // \' req
            postfix[k++] = p;
        else if(p == '*'){
            while(top >= 0 && op_stack[top] != '(' && op_stack[top] == '*'){
                postfix[k++] = op_stack[top];
                top--;
            }
            // after popping to postfix, push the *
            op_stack[++top] = p;
        }
         else if(p == '+'){
            while(top >= 0 && op_stack[top] != '(' && (op_stack[top] == '*' || op_stack[top] == '+')){
                postfix[k++] = op_stack[top];
                top--;
            }
            //after pop, push optr
            op_stack[++top] = p;
        }
        else if(p == '(')
            op_stack[++top] = p;
        else if(p == ')'){
            while(top >= 0 && op_stack[top] != '('){
                postfix[k++] = op_stack[top];
                top--;
            } top--;
        }
        i++;
    }

    while(top >= 0){
        char end = op_stack[top];
        if(end != '(')
            postfix[k++] = end;
        top--;
    }
    postfix[k] = '\0';
    free(op_stack);
    return postfix;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    if(expr == NULL)
        return NULL;
    //if(name == NULL)
     //   return NULL;
    
    char *postfix_expr = infix2postfix_sf(expr);
    // stack of pointers
    matrix_sf **ptr_stack = malloc(strlen(postfix_expr) * sizeof(matrix_sf*)); // pointers to mat stack
    int i = 0; 
    int top = -1; 
    int k = 0;

    while(postfix_expr[i] != '\0'){
        char p = postfix_expr[i];
        /* 
        As you evaluate a +, *, or ' operation, create a new matrix_sf and push
        it onto the stack. Do NOT add the new matrix to the BST.
        */
        if(p >= 'A' && p <= 'Z'){
            matrix_sf *matrix = find_bst_sf(p, root);
            ptr_stack[++top] = matrix;
        }
        else if(p == '\''){
            matrix_sf *matrix = ptr_stack[top];
            top--;
            matrix_sf *temp = transpose_mat_sf(matrix);
            temp->name = '^';
            // in a multi-operand function, there may be temporaries on the stack[top], 
            // free them if they are temporaries from 
            // previous operations that pushed matrices onto the stack
            if(matrix->name < 'A' || matrix->name > 'Z')
                free(matrix);
            ptr_stack[++top] = temp;
        }
        // since only one operation at a time, must check as previous could be temp, like (A + B) * C
        else if(p == '*'){
            matrix_sf *op2 = ptr_stack[top--];
            matrix_sf *op1 = ptr_stack[top--];
            matrix_sf *temp = mult_mats_sf(op1, op2);
            temp->name = '*';
            if(op1->name < 'A' || op1->name > 'Z')
                free(op1);
            if(op2->name < 'A' || op2->name > 'Z')
                free(op2);
            ptr_stack[++top] = temp;
        }
        else if(p == '+'){
            matrix_sf *op2 = ptr_stack[top--];
            matrix_sf *op1 = ptr_stack[top--];
            matrix_sf *temp = add_mats_sf(op1, op2);
            temp->name = '+';
            if(op1->name < 'A' || op1->name > 'Z')
                free(op1);
            if(op2->name < 'A' || op2->name > 'Z')
                free(op2);
            ptr_stack[++top] = temp;
        }
        i++;
    }
    matrix_sf *top_matrix = ptr_stack[top];
    top_matrix->name = name;
    free(ptr_stack);
    free(postfix_expr);
    return top_matrix;
}

matrix_sf *execute_script_sf(char *filename) {
    if(filename == NULL)
        return NULL;

    char *str = NULL;
    bst_sf *root = NULL;
    matrix_sf *returned_matrix  = NULL;
    FILE *file = fopen(filename, "r");
    if(file == NULL)
        return NULL;
    size_t max_line_size = MAX_LINE_LEN; 
    
    for(;;){
        if(getline(&str, &max_line_size, file) < 0) // no line
            break;
        char *p = str;
        if((*p < 'A' || *p > 'Z') && (*p < 'a' || *p > 'z') && *p != '\0')
            p++;
        // need end line check line 331 reference out, fake lines
        if(*p == '\0') continue;
        
        // ptr on start of matrix Name or expression start: A = 
        char matrix = *p++;
        while(*p != '=')
            p++;
        p++;
        while (*p == ' ')
            p++;

        if(*p >= '0' && *p <= '9') // matrices are created with two numbers first
            returned_matrix = create_matrix_sf(matrix, p);
        else // expression, doesnt initialize with digits 
            returned_matrix = evaluate_expr_sf(matrix, p, root);
        root = insert_bst_sf(returned_matrix, root);
    }
    free(str); fclose(file);
    return returned_matrix;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
