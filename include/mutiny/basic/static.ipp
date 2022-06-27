namespace mt {

constexpr u32 Status::get_error_num() const {
  return err_num;
}

constexpr u32 Status::get_warning_num() const {
  return warn_num;
}

} // namespace mt