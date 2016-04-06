//
//  client_main.cpp
//  sophos
//
//  Created by Raphael Bost on 03/04/2016.
//  Copyright © 2016 Raphael Bost. All rights reserved.
//

#include "sophos_client.hpp"
#include "logger.hpp"
#include "thread_pool.hpp"

#include <sse/dbparser/DBParserJSON.h>

#include <stdio.h>

//void load_inverted_index(sse::sophos::SophosClientRunner &runner, const std::string& path)
//{
//    sse::dbparser::DBParserJSON parser(path.c_str());
//    
//    auto add_pair_callback = [&runner](const string& keyword, const unsigned &doc)
//    {
////        std::cout << "Update: " << keyword << ", " << std::dec << doc << std::endl;
//        runner.update(keyword, doc);
//    };
//    
//    parser.addCallbackPair(add_pair_callback);
//    parser.parse();
//}

void load_inverted_index(sse::sophos::SophosClientRunner &runner, const std::string& path)
{
    sse::dbparser::DBParserJSON parser(path.c_str());
    ThreadPool pool(8);
    
    auto add_list_callback = [&runner,&pool](const string kw, const list<unsigned> docs)
    {
        //        std::cout << "Update: " << keyword << ", " << std::dec << doc << std::endl;
      
        auto work = [&runner](const string& keyword, const list<unsigned> &documents)
        {
            for (unsigned doc : documents) {
                runner.async_update(keyword, doc);
            }
        };
        pool.enqueue(work,kw,docs);
    };
    
    parser.addCallbackList(add_list_callback);
    parser.parse();
    
    runner.wait_updates_completion();
}

int main(int argc, char** argv) {
    // Expect only arg: --db_path=path/to/route_guide_db.json.
    sse::logger::set_severity(sse::logger::INFO);
    
    std::string save_path = "/Users/rbost/Code/sse/sophos/test.csdb";
    sse::sophos::SophosClientRunner client_runner("localhost:4242", save_path, 1e6, 1e5);
    
    std::vector<std::string> all_args;
    if (argc > 1) {
        all_args.assign(argv+1, argv+argc);
    }else{
        all_args.push_back("igualada");
    }
    
    if(client_runner.client().keyword_count() == 0)
    {
        // The database is empty, do some updates
        load_inverted_index(client_runner, "/Volumes/Storage/WP_Inverted/inverted_index_all_sizes/inverted_index_1000.json");
//        load_inverted_index(client_runner, "/Users/raphaelbost/Code/sse/sophos/inverted_index_test.json");
//        client_runner.update("dynamit", 0);
//        client_runner.update("dallacasa", 0);
//        client_runner.update("dallacasa", 2);
    }
    
    for (std::string &kw : all_args) {
        std::cout << "-------------- Search --------------" << std::endl;
        auto res = client_runner.search(kw);
        sse::logger::log(sse::logger::INFO) << "{";
        for (auto i : res) {
            sse::logger::log(sse::logger::INFO) << i << ", ";
        }
        sse::logger::log(sse::logger::INFO) << "}" << std::endl;
    }
//    std::cout << "-------------- Search --------------" << std::endl;
//    client_runner.search("dallacasa");
    
    
    return 0;
}