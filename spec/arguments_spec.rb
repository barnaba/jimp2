require 'siatkonator'

system "make siatkonator"
describe "siatkonator argument reader" do
  subject {Siatkonator.new}

  it "should require at least one option" do
    nator = subject.executed_with("")
    nator.status.should == :command_line_error
    nator.stderr.should match /error parsing command line options/i
  end

  it "should be ok when option given" do
    nator = subject.executed_with("spec/data/square.poly")
    nator.status.should == :OK
  end

  it "should fail when the only option is preceded with switch" do
    nator = subject.executed_with("-a 35")
    nator.status.should_not == :OK
  end

  it "should notify that max surface was set" do
    nator = subject.executed_with("-a 35 spec/data/square.poly")
    nator.status.should == :OK
    nator.stderr.should match /surface.*35/i
  end

  it "should notify that min angle was set" do
    nator = subject.executed_with("-q 20 spec/data/square.poly")
    nator.status.should == :OK
    nator.stderr.should match /Angle.*20/i
  end

  it "should notify that mesh files were provided" do
    nator = subject.executed_with("-e test.ele -e trele.ele -e morele.ele spec/data/square.poly")
    nator.status.should == :OK
    nator.stderr.should match /test\.ele/i
    nator.stderr.should match /trele\.ele/i
    nator.stderr.should match /morele\.ele/i
  end

  it "should notify that output file was set" do
    nator = subject.executed_with("-o out.ele spec/data/square.poly")
    nator.status.should == :OK
    nator.stderr.should match /saved.*out.ele/i
  end

end
