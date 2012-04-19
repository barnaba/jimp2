require 'open3'

class Siatkonator
  attr_reader :stdout, :stderr, :status
  ERRORS = {
    :OK => 0,
    :IO_error => 1,
    :file_format_error => 2,
    :command_line_error => 3
  }

  def executed_with(options)
    @stdout, @stderr, @process_status = Open3.capture3("./siatkonator #{options}")
    self
  end

  def status()
    ERRORS.invert[@process_status.exitstatus]
  end

  def has_point?(x, y, *attributes)
    @stderr =~ /Point\s+\d+:\s*#{x}\s*#{y}\s*attributes\s*#{attributes.join("\s*")}/ 
  end

  def has_segment?(a, b)
    @stderr =~ /Segment\s+\d+\s*points:\s*#{a}\s*#{b}/ 
  end

  def has_hole?(x, y)
    @stderr =~ /Hole\s*\d+:\s*#{x}\s*#{y}/ 
  end

end

