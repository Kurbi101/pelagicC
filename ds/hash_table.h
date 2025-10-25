#define HASH_TABLE_SIZE 67
#define HASH_TABLE_MAX_USAGE 0.7

#define DEFINE_HASH_TABLE(NAME, KEY, VAL, HASH, EQ)                                                                    \
	typedef struct NAME		   NAME;                                                                                   \
	typedef struct NAME##_node NAME##_node;                                                                            \
                                                                                                                       \
	struct NAME {                                                                                                      \
		u64			  len;                                                                                             \
		u64			  used;                                                                                            \
		NAME##_node **table;                                                                                           \
	};                                                                                                                 \
                                                                                                                       \
	struct NAME##_node {                                                                                               \
		KEY			 key;                                                                                              \
		VAL			 val;                                                                                              \
		NAME##_node *next;                                                                                             \
	};                                                                                                                 \
                                                                                                                       \
	NAME *NAME##_new() {                                                                                               \
		NAME *name = malloc(sizeof(NAME));                                                                             \
		if (!name) return NULL;                                                                                        \
		name->len	= HASH_TABLE_SIZE;                                                                                 \
		name->used	= 0;                                                                                               \
		name->table = calloc(name->len, sizeof(NAME##_node));                                                          \
		if (!name->table) {                                                                                            \
			free(name);                                                                                                \
			return NULL;                                                                                               \
		}                                                                                                              \
		return name;                                                                                                   \
	}                                                                                                                  \
                                                                                                                       \
	void NAME##_resize(NAME *table) {                                                                                  \
		u64			  new_len	= table->len * 2;                                                                      \
		NAME##_node **new_table = calloc(new_len, sizeof(NAME##_node));                                                \
		if (!new_table) return;                                                                                        \
                                                                                                                       \
		for (u64 i = 0; i < table->len; i++) {                                                                         \
			NAME##_node *node = table->table[i];                                                                       \
			while (node) {                                                                                             \
				NAME##_node *next  = node->next;                                                                       \
				u64			 index = HASH(&node->key) % new_len;                                                       \
				node->next		   = new_table[index];                                                                 \
				new_table[index]   = node;                                                                             \
				node			   = next;                                                                             \
			}                                                                                                          \
		}                                                                                                              \
                                                                                                                       \
		free(table->table);                                                                                            \
		table->table = new_table;                                                                                      \
		table->len	 = new_len;                                                                                        \
	}                                                                                                                  \
                                                                                                                       \
	void NAME##_insert(NAME *table, KEY key, VAL val) {                                                                \
		if (!table) {                                                                                                  \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		u64 index = HASH(&key) % table->len;                                                                           \
                                                                                                                       \
		if (table->table[index]) {                                                                                     \
			NAME##_node *new_node = malloc(sizeof(NAME##_node));                                                       \
			new_node->key		  = key;                                                                               \
			new_node->val		  = val;                                                                               \
			new_node->next		  = table->table[index];                                                               \
			table->table[index]	  = new_node;                                                                          \
		} else {                                                                                                       \
			table->table[index]		  = malloc(sizeof(NAME##_node));                                                   \
			table->table[index]->key  = key;                                                                           \
			table->table[index]->val  = val;                                                                           \
			table->table[index]->next = NULL;                                                                          \
			if ((double)(table->used++) / (double)(table->len) > HASH_TABLE_MAX_USAGE) {                               \
				NAME##_resize(table);                                                                                  \
			}                                                                                                          \
		}                                                                                                              \
	}                                                                                                                  \
                                                                                                                       \
	void NAME##_remove(NAME *table, KEY key) {                                                                         \
		if (!table) {                                                                                                  \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		u64 index = HASH(&key) % table->len;                                                                           \
                                                                                                                       \
		if (!table->table[index]) {                                                                                    \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		NAME##_node *node = table->table[index];                                                                       \
		NAME##_node *prev = NULL;                                                                                      \
                                                                                                                       \
		while (node && !EQ(&node->key, &key)) {                                                                        \
			NAME##_node *next = node->next;                                                                            \
			free(node);                                                                                                \
			node = next;                                                                                               \
		}                                                                                                              \
                                                                                                                       \
		if (!node) {                                                                                                   \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		if (!prev) {                                                                                                   \
			table->table[index] = node->next;                                                                          \
		} else {                                                                                                       \
			prev->next = node->next;                                                                                   \
		}                                                                                                              \
                                                                                                                       \
		free(node);                                                                                                    \
		table->used--;                                                                                                 \
	}                                                                                                                  \
                                                                                                                       \
	VAL NAME##_get(NAME *table, KEY key) {                                                                             \
		if (!table) {                                                                                                  \
			exit(EXIT_FAILURE);                                                                                        \
		}                                                                                                              \
                                                                                                                       \
		u64 index = HASH(&key) % table->len;                                                                           \
                                                                                                                       \
		if (!table->table[index]) {                                                                                    \
			/*TODO: add error handling */                                                                              \
			exit(EXIT_FAILURE);                                                                                        \
		}                                                                                                              \
                                                                                                                       \
		NAME##_node *node = table->table[index];                                                                       \
                                                                                                                       \
		while (node && !EQ(&node->key, &key)) {                                                                        \
			node = node->next;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		if (!node) {                                                                                                   \
			exit(EXIT_FAILURE);                                                                                        \
		}                                                                                                              \
                                                                                                                       \
		return node->val;                                                                                              \
	}                                                                                                                  \
                                                                                                                       \
	bool NAME##_contains(NAME *table, KEY key) {                                                                       \
		if (!table) {                                                                                                  \
			return false;                                                                                              \
		}                                                                                                              \
                                                                                                                       \
		u64 index = HASH(&key) % table->len;                                                                           \
                                                                                                                       \
		if (!table->table[index]) {                                                                                    \
			return false;                                                                                              \
		}                                                                                                              \
                                                                                                                       \
		NAME##_node *node = table->table[index];                                                                       \
                                                                                                                       \
		while (node && !EQ(&node->key, &key)) {                                                                        \
			node = node->next;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		return node != NULL;                                                                                           \
	}
