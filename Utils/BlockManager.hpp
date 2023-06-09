#ifndef BLOCKMANAGER_HPP
#define BLOCKMANAGER_HPP

#include <filesystem>
#include <fstream>
#include <optional>

//Поиск границы блока в файле с учетом выравнивания по строкам

struct BlockManager
{
    BlockManager(std::size_t blockCount_, std::string filePath):
        path{filePath},
        blockCount{blockCount_}
    {
        auto sz=std::filesystem::file_size(path);
        blockSize=sz/blockCount;
    }

    std::optional<std::size_t> getBlockEnd(std::size_t start) const
    {
        auto offset=nextEndLine(start+blockSize);
        return offset;
    }

    const auto getBlockCount() const    {   return blockCount;      }
    const auto getBlockSize() const     {   return blockSize;       }



private:
    std::filesystem::path path;
    std::size_t blockCount;
    std::size_t blockSize{0};

    std::optional<std::size_t> nextEndLine(std::size_t start) const
    {
        try
        {
            std::ifstream f{path};
            if(!f.is_open())    {   return std::nullopt;    }

            std::string temp;
            f.seekg(start);
            std::getline(f,temp);
            f.close();
            return (start+temp.size()+1);
        }
        catch(...)
        {
            return std::nullopt;
        }
    }
};

#endif // BLOCKMANAGER_HPP
