#pragma once

namespace io
{
	class serialized_object
	{
		friend class write_stream;
		friend class read_stream;

	protected:
		virtual void toStream(write_stream& wf) const = 0;
		virtual void fromStream(read_stream& rf) = 0;

	public:
		bool write(write_stream& wf) const
		{
			toStream(wf);
			// TODO: catch any exception, if so - return false;
			return true;
		}

		bool read(read_stream& rf)
		{
			fromStream(rf);
			// TODO: catch any exception, if so - return false;
			return true;
		}
	};
}