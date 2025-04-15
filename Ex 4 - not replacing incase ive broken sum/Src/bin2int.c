int binary_to_int(const char *bin_str) {
	int result = 0;
	while (*bin_str == '0' || *bin_str == '1') {
		result = (result << 1) | (*bin_str - '0');
		bin_str++;
	}
	return result;
}
