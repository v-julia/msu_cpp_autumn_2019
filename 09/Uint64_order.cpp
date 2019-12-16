
#include "Uint64_order.h"



void Uint64_order::sort_uint64(std::ifstream& for_sort)
{

    std::unique_ptr<uint64_t[]> dat(new uint64_t[cnt_block]);
    size_t sz = sizeof(uint64_t);
    while ( !for_sort.eof() ) {
        size_t cnt = 0;
        {
            std::lock_guard<std::mutex> lock(guard_input);
            while ( cnt < cnt_block )
                if ( !for_sort.read(reinterpret_cast<char*>( &dat[cnt] ), sz) )
                    break;
                else
                    ++cnt;
        }

        // упорядочить dat
        if ( cnt > 0 ) {
            qsort(static_cast<void*>( dat.get() ), cnt, sz, [](const void* a, const void* b) {
                uint64_t arg1 = *static_cast<const uint64_t*>( a );
                uint64_t arg2 = *static_cast<const uint64_t*>( b );
                if ( arg1 < arg2 ) return -1;
                if ( arg1 > arg2 ) return 1;
                return 0;

            });

            // проверить очередь, узнать какую часть взять для обработки
            bool is_empty = true;
            std::string file_name_from_queue;
            {
                std::lock_guard<std::mutex> lock(guard_queue_parts);
                is_empty = queue_parts.empty();
                if ( !is_empty ) {
                    file_name_from_queue = queue_parts.front();
                    queue_parts.pop();
                }
            }

            // получить номер частичного файла 
            std::string file_name_merged;
            {
                std::lock_guard<std::mutex> lock(guard_nom_part);
                file_name_merged = "part_" + std::to_string(nom_part++);
            }

            // записать в файл
            if ( is_empty ) {
                std::ofstream sorted_part;
                sorted_part.open(file_name_merged, std::ios::binary);
                for ( size_t i = 0; i < cnt; ++i ) {
                    sorted_part.write(reinterpret_cast<char*>( &dat[i] ), sz);
                }
                sorted_part.close();
            }
            else { // merge dat и файл из очереди
                merge_array_with_part(dat, cnt, file_name_from_queue, file_name_merged);
            }

            // поместить в очередь
            {
                std::lock_guard<std::mutex> lock(guard_queue_parts);
                queue_parts.emplace(file_name_merged);
            }
        }
    }
    while ( true ) {
        size_t sz_que;
        std::string file_part1_name;
        std::string file_part2_name;
        {
            std::lock_guard<std::mutex> lock(guard_queue_parts);
            sz_que = queue_parts.size();
            if ( sz_que > 1 ) {
                file_part1_name = queue_parts.front();
                queue_parts.pop();
                file_part2_name = queue_parts.front();
                queue_parts.pop();
            }
        }
        if ( sz_que > 1 ) {

            std::string file_name_merged;
            {
                std::lock_guard<std::mutex> lock(guard_nom_part);
                file_name_merged = "part_" + std::to_string(nom_part++);
            }

            // слить два файла в один
            merge_parts(file_part1_name, file_part2_name, file_name_merged);

            // поместить результат в очередь
            {
                std::lock_guard<std::mutex> lock(guard_queue_parts);
                queue_parts.emplace(file_name_merged);
            }

        }
        else {
            break;
        }
    }
}


void Uint64_order::merge_parts(std::string& p1fn, std::string& p2fn, std::string& outfn)
{
    std::ifstream part1file;
    std::ifstream part2file;
    std::ofstream outfile;
    part1file.open(p1fn, std::ios::binary);
    part2file.open(p2fn, std::ios::binary);
    outfile.open(outfn, std::ios::binary);

    uint64_t value1;
    uint64_t value2;
    size_t sz = sizeof(uint64_t);

    bool stop_part1 = !part1file.read(reinterpret_cast<char*>( &value1 ), sz);;
    bool stop_part2 = !part2file.read(reinterpret_cast<char*>( &value2 ), sz);;

    while ( !( stop_part1 && stop_part2 ) ) {
        if ( !stop_part1 && !stop_part2 ) {
            if ( value1 < value2 ) {
                outfile.write(reinterpret_cast<char*>( &value1 ), sz);
                stop_part1 = !part1file.read(reinterpret_cast<char*>( &value1 ), sz);;
            }
            else {
                outfile.write(reinterpret_cast<char*>( &value2 ), sz);
                stop_part2 = !part2file.read(reinterpret_cast<char*>( &value2 ), sz);;
            }
        }
        if ( !stop_part1 && stop_part2 ) {
            outfile.write(reinterpret_cast<char*>( &value1 ), sz);
            stop_part1 = !part1file.read(reinterpret_cast<char*>( &value1 ), sz);;
        }
        if ( stop_part1 && !stop_part2 ) {
            outfile.write(reinterpret_cast<char*>( &value2 ), sz);
            stop_part2 = !part2file.read(reinterpret_cast<char*>( &value2 ), sz);;
        }
    }
    part1file.close();
    part2file.close();
    outfile.close();
    remove(p1fn.c_str()); // delete file
    remove(p2fn.c_str());
}

void Uint64_order::merge_array_with_part(std::unique_ptr<uint64_t[]>& dat, size_t cnt, std::string& pfn, std::string& outfn)
{
    std::ifstream partfile;
    std::ofstream outfile;
    partfile.open(pfn, std::ios::binary);
    outfile.open(outfn, std::ios::binary);

    uint64_t value;
    size_t sz = sizeof(uint64_t);

    bool stop_dat = ( cnt == 0 );
    size_t idx = 0;
    bool stop_part = !partfile.read(reinterpret_cast<char*>( &value ), sz);

    while ( !( stop_dat && stop_part ) ) {
        if ( !stop_dat && !stop_part ) {
            if ( dat[idx] < value ) {
                outfile.write(reinterpret_cast<char*>( &dat[idx++] ), sz);
                stop_dat = ( idx >= cnt );
            }
            else {
                outfile.write(reinterpret_cast<char*>( &value ), sz);
                stop_part = !partfile.read(reinterpret_cast<char*>( &value ), sz);;
            }
        }
        if ( !stop_dat && stop_part ) {
            outfile.write(reinterpret_cast<char*>( &dat[idx++] ), sz);
            stop_dat = ( idx >= cnt );
        }
        if ( stop_dat && !stop_part ) {
            outfile.write(reinterpret_cast<char*>( &value ), sz);
            stop_part = !partfile.read(reinterpret_cast<char*>( &value ), sz);;
        }
    }
    partfile.close();
    outfile.close();
    remove(pfn.c_str()); // delete file
}
