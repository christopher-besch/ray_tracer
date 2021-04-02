#pragma once

#include <string>

#include "utils.h"

const int alphabet_size = 26;

struct TrieNode
{
    // when this is leaf node
    bool end_of_word;
    // either nullptr or pointing to next node
    TrieNode *children[alphabet_size];

    TrieNode()
        : end_of_word(false)
    {
        for (int idx = 0; idx < alphabet_size; idx++)
            children[idx] = nullptr;
    }
    TrieNode(const TrieNode &other)
    {
        for (int idx = 0; idx < alphabet_size; idx++)
        {
            end_of_word = other.end_of_word;
            if (other.children[idx])
                children[idx] = new TrieNode(*other.children[idx]);
            else
                children[idx] = nullptr;
        }
    }
    TrieNode(const TrieNode &&other)
    {
        for (int idx = 0; idx < alphabet_size; idx++)
        {
            end_of_word = other.end_of_word;
            children[idx] = std::move(other.children[idx]);
        }
    }
    TrieNode &operator=(const TrieNode &other)
    {
        for (int idx = 0; idx < alphabet_size; idx++)
        {
            delete children[idx];
            end_of_word = other.end_of_word;
            if (other.children[idx])
                children[idx] = new TrieNode(*other.children[idx]);
            else
                children[idx] = nullptr;
        }
        return *this;
    }
    TrieNode &operator=(const TrieNode &&other)
    {
        for (int idx = 0; idx < alphabet_size; idx++)
        {
            delete children[idx];
            end_of_word = other.end_of_word;
            children[idx] = std::move(other.children[idx]);
        }
        return *this;
    }

    ~TrieNode()
    {
        for (int idx = 0; idx < alphabet_size; idx++)
            delete children[idx];
    }
};

class Trie
{
private:
    TrieNode *m_root;

private:
    int char_to_idx(char character) const
    {
        if (character < 'a' || character > 'z')
            raise_error(character << "is an invalid character for Trie!");
        return character - 'a';
    }

public:
    Trie()
        : m_root(new TrieNode) {}
    Trie(const Trie &other)
        : m_root(new TrieNode(*other.m_root)) {}
    Trie(const Trie &&other)
        : m_root(std::move(other.m_root)) {}
    Trie &operator=(const Trie &other)
    {
        delete m_root;
        m_root = new TrieNode(*other.m_root);
        return *this;
    }
    Trie &operator=(const Trie &&other)
    {
        delete m_root;
        m_root = std::move(other.m_root);
        return *this;
    }

    ~Trie()
    {
        delete m_root;
    }

    void insert(std::string new_key);
    bool search(std::string key) const;
};

// todo: does this work?
