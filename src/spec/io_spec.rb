require_relative 'siatkonator'

system "make siatkonator"
describe "siatkonator IO" do

  context "when it reads square.poly" do
    subject {Siatkonator.new.executed_with("spec/data/square.poly") }

    it "should read poly file without error" do
      subject.status.should == :OK
      subject.stderr.should match /read 4 nodes/i
      subject.stderr.should match /read 4 segments/i
      subject.stderr.should match /read 2 holes/i
    end

    it "should read nodes coordinates and attributes properly" do
      subject.should have_point 1, 1, 1.2
      subject.should have_point 1, 2, 2.2
      subject.should have_point 2, 2, 3.3
      subject.should have_point 2, 1, 3.14
    end

    it "should read nodes segments and holes properly" do
      subject.should have_segment 0, 1
      subject.should have_segment 1, 2
      subject.should have_segment 2, 3
      subject.should have_segment 3, 0
      subject.should have_hole 1.5, 1.5
      subject.should have_hole 1.7, 1.2
    end
  end

  context "when it reads square-firstnode-1.poly" do
    subject {Siatkonator.new.executed_with("spec/data/square-firstnode-1.poly") }

    it "should read poly file without error" do
      subject.status.should == :OK
      subject.stderr.should match /read 4 nodes/i
      subject.stderr.should match /read 4 segments/i
      subject.stderr.should match /read 2 holes/i
    end

    it "should read nodes coordinates and attributes properly" do
      subject.should have_point 1, 1, 1.2
      subject.should have_point 1, 2, 2.2
      subject.should have_point 2, 2, 3.3
      subject.should have_point 2, 1, 3.14
    end

    it "should read nodes segments and holes properly" do
      subject.should have_segment 0, 1
      subject.should have_segment 1, 2
      subject.should have_segment 2, 3
      subject.should have_segment 3, 0
      subject.should have_hole 1.5, 1.5
      subject.should have_hole 1.7, 1.2
    end
  end

end
