#include "../../include/commands/write-tree.hpp"
#include "../../include/objects/tree.hpp"

#include <algorithm>
#include <iostream>
#include <set>
#include <map>

struct DirNode
{
    std::map<std::string, DirNode> subdirs;
    std::vector<TreeEntry> blobs;
};

void addPath(DirNode &root, std::filesystem::path path, std::string hash)
{
    DirNode *node = &root;
    for (auto it = path.begin(); it != path.end(); ++it)
    {
        bool last = (std::next(it) == path.end());
        if (last)
        {
            node->blobs.push_back(TreeEntry{*it, hash, TreeEntryType::Blob});
        }
        else
        {
            std::string key = it->string();
            node = &node->subdirs[key];
        }
    }
}

Tree buildTree(DirNode &root, Object_store &object_store)
{
    std::vector<TreeEntry> entries = root.blobs;

    for (std::pair<const std::string, DirNode> &subdir_entry : root.subdirs)
    {
        const std::string dirname = subdir_entry.first;
        DirNode &child = subdir_entry.second;

        Tree childTree = buildTree(child, object_store);
        if (object_store.write_object(childTree) == -1)
        {
            return Tree({});
        }

        std::string childHash = childTree.getHash();

        entries.push_back(TreeEntry{
            std::filesystem::path(dirname),
            childHash,
            TreeEntryType::Tree});
    }

    Tree root_tree = Tree(entries);
    return root_tree;
}

int Write_tree::execute()
{
    std::vector<Index_entry> index_entries = repository.index().load();
    DirNode root;

    for (Index_entry index_entry : index_entries)
    {
        addPath(root, index_entry.path, index_entry.object_hash);
    }

    Tree root_tree = buildTree(root, repository.objects());

    if (repository.objects().write_object(root_tree) == -1)
    {
        return -1;
    }

    std::cout << root_tree.getHash() << '\n';
    return 0;
}
