#pragma once

namespace io
{
	class serialized_object
	{
		friend class write_stream;
		friend class read_stream;

	protected:
		virtual void to_stream(write_stream& wf) const = 0;
		virtual void from_stream(read_stream& rf) = 0;

	public:
		bool write(write_stream& wf) const
		{
			to_stream(wf);
			// TODO: catch any exception, if so - return false;
			return true;
		}

		bool read(read_stream& rf)
		{
			from_stream(rf);
			// TODO: catch any exception, if so - return false;
			return true;
		}
	};
}