namespace mt {

constexpr u32 Status::get_error_num() const {
  return err_num;
}

constexpr u32 Status::get_warning_num() const {
  return warn_num;
}

constexpr Logger& Status::get_log_out() {
  return log_out;
}

constexpr Logger& Status::get_log_err() {
  return log_err;
}

} // namespace mt