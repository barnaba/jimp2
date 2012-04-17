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

end

describe "siatkonator IO" do
  before :all do
    system "make siatkonator"
  end
  subject {Siatkonator.new}

  it "should read poly file" do
    poly = 'square.poly'
    nator = subject.executed_with("spec/data/#{poly}")
    nator.status.should == :OK
    nator.stderr.should match /reading #{poly}/i
    nator.stderr.should match /read 4 nodes/i
    #nator.stderr.should match /#{poly}: read 4 segments/i
  end

  it "should read nodes coordinates and attributes properly" do
    poly = 'square.poly'
    nator = subject.executed_with("spec/data/#{poly}")
    nator.status.should == :OK
    nator.stderr.should match /Point    0:  1  1   attributes  1.2/
    nator.stderr.should match /Point    1:  1  2   attributes  2.2/
    nator.stderr.should match /Point    2:  2  2   attributes  3.3/
    nator.stderr.should match /Point    3:  2  1   attributes  3.14/
  end

end
