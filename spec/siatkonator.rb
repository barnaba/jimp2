require 'open3'

class Siatkonator
  attr_reader :stdout, :stderr, :status, :current
  ERRORS = {
    :OK => 0,
    :IO_error => 1,
    :file_format_error => 2,
    :command_line_error => 3
  }

  def executed_with(options)
    bin = File.dirname(__FILE__) + "/../bin/siatkonator"
    @stdout, @stderr, @process_status = Open3.capture3("#{bin} #{options}")
    @current = @stdout.split("\n") + @stderr.split("\n")
    puts "executing: #{bin} #{options}"
    self
  end

  def status()
    ERRORS.invert[@process_status.exitstatus]
  end

  def has_point?(x, y, *attributes)
    matching? /Point\s+\d+:\s*#{x}\s*#{y}\s*#{attributes.empty? ? "" : "attributes"}\s*#{attributes.join("\s*")}/ 
  end

  def has_segment?(a, b)
    matching? /Segment\s+\d+\s*points:\s*#{a}\s*#{b}/ 
  end

  def has_hole?(x, y)
    matching? /Hole\s*\d+:\s*#{x}\s*#{y}/ 
  end

  def has_triangle?(n1, n2, n3)
    matching? /Triangle\s+\d+\s+points:\s+#{n1}\s+#{n2}\s+#{n3}/ 
  end

  def set_section!(start, stop=/--------/)
    start = Regexp.new("\\*\\*\\*.*" + start.source)
    @current = @current.drop_while { |line| not line =~ start }.take_while {|line| not line =~ stop}
  end

  def matching?(regex)
    not @current.select { |line| line =~ regex }.empty?
  end

end

