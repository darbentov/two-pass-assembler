Opcode mov = {'mov', '0000', OPS_2};
Opcode cmp = {'cmp', '0001', OPS_2};
Opcode add = {'add', '0010', OPS_2};
Opcode sub = {'sub', '0011', OPS_2};
Opcode not = {'not', '0100', OP_1};
Opcode clr = {'clr', '0101', OP_1};
Opcode lea = {'lea', '0110', OPS_2};
Opcode inc = {'inc', '0111', OP_1};
Opcode dec = {'dec', '1000', OP_1};
Opcode jmp = {'jmp', '1001', OP_1};
Opcode bne = {'bne', '1010', OP_1};
Opcode red = {'red', '1011', OP_1};
Opcode prn = {'prn', '1100', OP_1};
Opcode jsr = {'jsr', '1101', OP_1};
Opcode rts = {'rts', '1110', NO_OP};
Opcode stop = {'stop', '1111', NO_OP};

int getHashIndex(char *name) {
    int sum;
    char c;
    while ((c = *(name++))) {
        sum += c;
    }
    return sum % OPCODES_COUNT;
}

void init_opcode_hash_table() {
    insertOpCodeNode(&mov);
    insertOpCodeNode(&cmp);
    insertOpCodeNode(&add);
    insertOpCodeNode(&sub);
    insertOpCodeNode(&not);
    insertOpCodeNode(&clr);
    insertOpCodeNode(&lea);
    insertOpCodeNode(&inc);
    insertOpCodeNode(&dec);
    insertOpCodeNode(&jmp);
    insertOpCodeNode(&bne);
    insertOpCodeNode(&red);
    insertOpCodeNode(&prn);
    insertOpCodeNode(&jsr);
    insertOpCodeNode(&rts);
    insertOpCodeNode(&stop);
}

void insertOpCodeNode(Opcode *node, int index) {
    if (hash_table[index] == NULL) {
        hash_table[index] = node;
        node->next = NULL;
    } else {
        Opcode *temp = hash_table[index];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = Node;
        Node->next = NULL;
    }
}

void insertIntoHashMap(Opcode *Node) {
    int index = getHashIndex(Node->name);
    insertOpCodeNode(Node, index);
}

Opcode *getOpcodeNode(char *op) {
    Opcode *temp = NULL;
    int index = getHashIndex(op);
    if (hash_table[index] != NULL) {
        temp = hash_table[index];
        while (strcmp(temp->name, op) && temp != NULL) {
            temp = temp->next;
        }
    }

    if (temp == NULL) {
        printf("Opcode '%s' was not found!", op);
        return NULL;
    } else {
        return temp;
    }
}
