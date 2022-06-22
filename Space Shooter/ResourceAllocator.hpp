#ifndef ResourceAllocator_hpp
#define ResourceAllocator_hpp

#include <map>
#include <memory>
#include <string>

template<typename T>
class ResourceAllocator
{
public:
    int add(const std::string& filePath)
    {
        auto it = m_resources.find(filePath);
        if (it != m_resources.end())
        {
            return it->second.first;
        }

        std::shared_ptr<T> resource = std::make_shared<T>();
        if (!resource->loadFromFile(filePath))
        {
            return -1;
        }

        m_resources.insert(std::make_pair(filePath, std::make_pair(m_currentId, resource)));

        return m_currentId++;
    }

    void remove(int id)
    {
        for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
        {
            if (it->second.first == id)
            {
                resources.erase(it->first);
            }
        }
    }

    std::shared_ptr<T> get(int id)
    {
        for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
        {
            if (it->second.first == id)
            {
                return it->second.second;
            }
        }

        return nullptr;
    }

    bool has(int id)
    {
        return (get(id) != nullptr);
    }

private:
    int m_currentId;
    std::map<std::string, std::pair<int, std::shared_ptr<T>>> m_resources;
};

#endif