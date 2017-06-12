/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Yu Jing <yu@argcv.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#include <cstdio>

#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <vector>

#include "glog/logging.h"
#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/write_batch.h"

typedef bool (*kv_handler)(const std::string&, const std::string&, void*);

::leveldb::DB* ldb_init(const std::string& ddir, ::leveldb::Options* _opt,
                        size_t cache_size = 0, bool create_if_missing = true);
bool ldb_close(::leveldb::DB* db, ::leveldb::Options* _opt);

bool ldb_start_with(::leveldb::DB* db, const std::string& base, kv_handler kvs,
                    void* data = nullptr);
bool ldb_exist(::leveldb::DB* db, const std::string& key);
bool ldb_put(::leveldb::DB* db, const std::string& key,
             const std::string& val = "");
bool ldb_get(::leveldb::DB* db, const std::string& key, std::string* _val);
bool ldb_rm(::leveldb::DB* db, const std::string& key);

bool ldb_batch_put(::leveldb::DB* db,
                   const std::map<std::string, std::string>& kvs);
bool ldb_batch_rm(::leveldb::DB* db, const std::set<std::string>& keys);

bool ldb_destroy(const std::string& ddir);

::leveldb::DB* ldb_init(const std::string& ddir, ::leveldb::Options* _opt,
                        size_t cache_size, bool create_if_missing) {
  if (_opt == nullptr) {
    return nullptr;
  }
  _opt->create_if_missing = create_if_missing;
  if (cache_size > 0) {
    _opt->block_cache = ::leveldb::NewLRUCache(cache_size);
  }
  ::leveldb::DB* db;
  return ::leveldb::DB::Open(*_opt, ddir, &db).ok() ? db : nullptr;
}

bool ldb_close(::leveldb::DB* db, ::leveldb::Options* _opt) {
  if (db != nullptr) {
    delete db;
  }
  if (_opt != nullptr && _opt->block_cache != nullptr) {
    delete _opt->block_cache;
  }
  return true;
}

bool ldb_start_with(::leveldb::DB* db, const std::string& base, kv_handler kvs,
                    void* data) {
  ::leveldb::ReadOptions snap_read_opt;
  snap_read_opt.snapshot = db->GetSnapshot();
  ::leveldb::Iterator* it = db->NewIterator(snap_read_opt);
  ::leveldb::Slice slice = base;
  for (it->Seek(slice);
       it->Valid() &&
       std::mismatch(base.begin(), base.end(), it->key().ToString().begin())
               .first == base.end();
       it->Next()) {
    if (!kvs(it->key().ToString(), it->value().ToString(), data)) {
      break;
    }
  }
  delete it;
  db->ReleaseSnapshot(snap_read_opt.snapshot);
  return true;
}

bool ldb_exist(::leveldb::DB* db, const std::string& key) {
  std::string val;
  return ((db->Get(::leveldb::ReadOptions(), key, &val)).ok());
}

bool ldb_put(::leveldb::DB* db, const std::string& key,
             const std::string& val) {
  return ((db->Put(::leveldb::WriteOptions(), key, val)).ok());
}

bool ldb_get(::leveldb::DB* db, const std::string& key, std::string* _val) {
  return ((db->Get(::leveldb::ReadOptions(), key, _val)).ok());
}

bool ldb_rm(::leveldb::DB* db, const std::string& key) {
  return ((db->Delete(::leveldb::WriteOptions(), key)).ok());
}

bool ldb_batch_put(::leveldb::DB* db,
                   const std::map<std::string, std::string>& kvs) {
  ::leveldb::WriteBatch* _wb = new ::leveldb::WriteBatch;
  for (std::map<std::string, std::string>::const_iterator it = kvs.begin();
       it != kvs.end(); it++) {
    // printf("ldb_batch_add %s -> %s\n", it->first.c_str(),
    // it->second.c_str());
    _wb->Put(it->first, it->second);
  }
  bool rst = ((db->Write(::leveldb::WriteOptions(), _wb)).ok());
  delete _wb;
  return rst;
}

bool ldb_batch_rm(::leveldb::DB* db, const std::set<std::string>& keys) {
  ::leveldb::WriteBatch* _wb = new ::leveldb::WriteBatch;
  for (std::set<std::string>::const_iterator it = keys.begin();
       it != keys.end(); it++) {
    _wb->Delete(*it);
  }
  bool rst = ((db->Write(::leveldb::WriteOptions(), _wb)).ok());
  delete _wb;
  return rst;
}

bool ldb_destroy(const std::string& ddir) {
  return ::leveldb::DestroyDB(ddir, ::leveldb::Options()).ok();
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  ::leveldb::Options opt;
  std::string ddir = "/tmp/ldb-test";
  size_t cache_size = 1000;
  bool create_if_missing = true;

  ::leveldb::DB* db = ldb_init(ddir, &opt, cache_size, create_if_missing);

  // bool ldb_exist(::leveldb::DB* db, const std::string& key);
  // bool ldb_put(::leveldb::DB* db, const std::string& key,
  //              const std::string& val = "");
  // bool ldb_get(::leveldb::DB* db, const std::string& key, std::string* _val);
  // bool ldb_rm(::leveldb::DB* db, const std::string& key);

  ldb_put(db, "alice::alice", "1");
  ldb_put(db, "alice::bob", "2");
  ldb_put(db, "alice::carol", "3");

  LOG(INFO) << "check exists: alice::bob: "
            << (ldb_exist(db, "alice::bob") ? "Found" : "Not Found");

  std::string val_ab;

  if (ldb_get(db, "alice::bob", &val_ab)) {
    LOG(INFO) << "get alice::bob: " << val_ab;
  }

  ldb_rm(db, "alice::bob");

  // bool ldb_start_with(::leveldb::DB* db, const std::string& base, kv_handler
  // kvs,
  //                     void* data = nullptr);
  ldb_start_with(db, "alice::",
                 [](const std::string& k, const std::string& v, void*) -> bool {
                   LOG(INFO) << "key:" << k << ", v:" << v;
                   return true;
                 });

  ldb_close(db, &opt);
  ldb_destroy(ddir);
  return 0;
}
