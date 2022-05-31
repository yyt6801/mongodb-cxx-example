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
	// ֻ����һ��mongocxx::instanceʵ��
	bool Intialize(std::unique_ptr<mongocxx::instance> instance,
		std::unique_ptr<mongocxx::pool> pool);
	// �����ӳ��л�ȡ����
	mongocxx::pool::entry get_connection();
	// ���Ի�ȡ�������ӣ����̺߳�Э�̻�������ʹ��
	mongocxx::stdx::optional<mongocxx::pool::entry> try_get_connection();
	// ��ȡ���ݿ�
	mongocxx::database get_database(std::string db_name);


	// ����һ��
	mongocxx::stdx::optional<mongocxx::result::insert_one> insert_one(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value doc, mongocxx::options::insert options = {});
	// ������
	mongocxx::stdx::optional<mongocxx::result::insert_many> insert_many(std::string db_name, std::string coll_name, 
		std::vector<bsoncxx::document::value> docs, mongocxx::options::insert options = {});
	//time_t convert_string_to_time_t(const std::string& time_string);


	// ��ѯ
	mongocxx::stdx::optional<mongocxx::cursor> find(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, mongocxx::options::find options = {});

	mongocxx::stdx::optional<bsoncxx::document::value> find_one(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, mongocxx::options::find options = {});


	// ����
	mongocxx::stdx::optional<mongocxx::result::update> update_one(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, bsoncxx::document::view_or_value update_value, mongocxx::options::update options = {});

	mongocxx::stdx::optional<mongocxx::result::update> update_many(std::string db_name, std::string coll_name, 
		bsoncxx::document::view_or_value filter, bsoncxx::document::view_or_value update_value, mongocxx::options::update options = {});


	// ɾ��
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