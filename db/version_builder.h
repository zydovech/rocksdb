//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
#pragma once
#include "rocksdb/file_system.h"
#include "rocksdb/slice_transform.h"

namespace ROCKSDB_NAMESPACE {

class TableCache;
class VersionStorageInfo;
class VersionEdit;
struct FileMetaData;
class InternalStats;

// A helper class so we can efficiently apply a whole sequence
// of edits to a particular state without creating intermediate
// Versions that contain full copies of the intermediate state.
//一个帮助类，用于合并多个version edit到一个version，省的中间创建很多version
class VersionBuilder {
 public:
  VersionBuilder(const FileOptions& file_options, TableCache* table_cache,
                 VersionStorageInfo* base_vstorage, Logger* info_log = nullptr);
  ~VersionBuilder();
  Status CheckConsistency(VersionStorageInfo* vstorage);
  Status CheckConsistencyForDeletes(VersionEdit* edit, uint64_t number,
                                    int level);
  bool CheckConsistencyForNumLevels();
  //应用一个edit到builder里面
  Status Apply(VersionEdit* edit);
  //把现有的版本，saveto到storage info
  Status SaveTo(VersionStorageInfo* vstorage);
  Status LoadTableHandlers(InternalStats* internal_stats, int max_threads,
                           bool prefetch_index_and_filter_in_cache,
                           bool is_initial_load,
                           const SliceTransform* prefix_extractor);
  void MaybeAddFile(VersionStorageInfo* vstorage, int level, FileMetaData* f);

 private:
  class Rep;
  Rep* rep_;
};

extern bool NewestFirstBySeqNo(FileMetaData* a, FileMetaData* b);
}  // namespace ROCKSDB_NAMESPACE
