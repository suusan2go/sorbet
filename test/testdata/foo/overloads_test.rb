# typed: true

class A
  extend T::Sig

  sig { params(blk: T.proc.params(x: Integer, y: Integer).void).returns(Integer) }
  sig { params(blk: T.proc.params(x: String).void).returns(String) }
  def self.example(&blk)
  end
end

A.example do |x, y|
  T.reveal_type(x)
  T.reveal_type(y)
end

A.example do |x|
  T.reveal_type(x)
end

A.example
