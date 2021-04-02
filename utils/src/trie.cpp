#include "trie.h"

void Trie::insert(std::string key)
{
    // start at root node
    TrieNode *current_node = m_root;
    for (int idx = 0; idx < key.size(); idx++)
    {
        int char_idx = char_to_idx(key[idx]);
        // when not found
        if (!current_node->children[char_idx])
            current_node->children[char_idx] = new TrieNode;
        current_node = current_node->children[char_idx];
    }
    // last node is leaf
    current_node->end_of_word = true;
}

// return true if found
bool Trie::search(std::string key) const
{
    // start at root node
    TrieNode *current_node = m_root;
    for (int idx = 0; idx < key.size(); idx++)
    {
        int char_idx = char_to_idx(key[idx]);
        // when not found
        if (!current_node->children[char_idx])
            return false;
        current_node = current_node->children[char_idx];
    }
    // only when existent and leaf
    return current_node && current_node->end_of_word;
}
