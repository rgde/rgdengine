#pragma once

namespace rgde
{
	namespace game
	{
		class object;

		typedef std::list<object*> objects_list;
		typedef objects_list::iterator obj_iter;

		typedef obj_iter handle;


		class system 
		{
			friend object;

		public:
			system();
			~system();

			virtual void do_update(float dt);

			static system* get();

		private:
			handle add(object* obj);
			void remove(handle h);

		public:
			static system* ms_instance;
			objects_list m_list;
		};

		class object
		{
			friend system;
		protected:
			object();
			~object();

			virtual void update(float dt) = 0;

		private:
			handle m_handle;
		};
	}
}
