#pragma once
const struct SockConfigure {
	struct  {
		const char* localhost = "127.0.0.1";
		const int port = 1111;
	} net_properties;
	struct {
		const int max_client_count = 10;
	} additional_properties;
} sockConfigure;