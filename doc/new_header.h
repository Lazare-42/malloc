

struct s_node_pointer_container {
    void                *ptr_address_;
    void                *ptr_node_left_;
    void                *ptr_node_right_;
}

struct s_block {
    uint64_t            u64_size_;
    uint64_t            u64_free_size_;
    uint8_t             *u8_memory_;
    struct s_block      *ptr_next_;
    struct s_page       *ptr_page_base_;
};

struct s_page {
    uint64_t            u64_size_;
    uint64_t            u64_block_size_; 
    uint64_t            u64_number_of_used_blocks_in_page_;
    struct s_block      *ptr_first_free_block_;
    struct s_page       *ptr_next_page_same_category_;
    struct s_page       *ptr_next_page_upper_category_;
};
