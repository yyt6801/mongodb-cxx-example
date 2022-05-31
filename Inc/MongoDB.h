#ifndef MongoDB_H
#define MongoDB_H
#include <vector>
#include <string>

#include <mongocxx\instance.hpp>
#include <mongocxx\pool.hpp>
#include <mongocxx\client.hpp>
#include <mongocxx\stdx.hpp>
#include <mongocxx\uri.hpp>

class MongoDB
{
public:
	MongoDB(std::string uri_string);
	~MongoDB();
	// 只能有一个mongocxx::instance实例
	bool Intialize(std::unique_ptr<mongocxx::instance> instance,
		std::unique_ptr<mongocxx::pool> pool);
	// 从连接池中获取连接
	mongocxx::pool::entry get_connection();
	// 尝试获取可用连接，多线程和协程环境必须使用
	mongocxx::stdx::optional<mongocxx::pool::entry> try_get_connection();
	// 获取数据库
	mongocxx::database get_database(std::string db_name);


	// 插入一个
	mongocxx::stdx::optional<mongocxx::result::insert_one> insert_one(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value doc, mongocxx::options::insert options = {});
	// 插入多个
	mongocxx::stdx::optional<mongocxx::result::insert_many> insert_many(std::string db_name, std::string coll_name, 
		std::vector<bsoncxx::document::value> docs, mongocxx::options::insert options = {});
	//time_t convert_string_to_time_t(const std::string& time_string);


	// 查询
	mongocxx::stdx::optional<mongocxx::cursor> find(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, mongocxx::options::find options = {});

	mongocxx::stdx::optional<bsoncxx::document::value> find_one(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, mongocxx::options::find options = {});


	// 更新
	mongocxx::stdx::optional<mongocxx::result::update> update_one(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, bsoncxx::document::view_or_value update_value, mongocxx::options::update options = {});

	mongocxx::stdx::optional<mongocxx::result::update> update_many(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, bsoncxx::document::view_or_value update_value, mongocxx::options::update options = {});


	// 删除
	mongocxx::stdx::optional<mongocxx::result::delete_result> delete_one(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, mongocxx::options::delete_options options = {});

	mongocxx::stdx::optional<mongocxx::result::delete_result> delete_many(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, mongocxx::options::delete_options options = {});


	// GridFS
	mongocxx::stdx::optional<mongocxx::result::gridfs::upload> upload_file_to_gridfs(std::string file_name,
		std::string file_path, std::string db_name, mongocxx::options::gridfs::bucket bucket_option = {}, mongocxx::options::gridfs::upload upload_options = {});

	bool download_file_from_gridfs(std::string db_name, bsoncxx::types::value file_id, std::string storage_path,
		mongocxx::options::gridfs::bucket bucket_options = {});

private:
	std::unique_ptr<mongocxx::instance> _instance = nullptr;
	std::unique_ptr<mongocxx::pool> _pool = nullptr;
};

#endif