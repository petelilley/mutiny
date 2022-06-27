namespace mt {

template<typename T>
constexpr T Token::get_value() const {
  return std::get<T>(value);
}

} // namespace mt