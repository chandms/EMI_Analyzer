original_message = 'This is a very long long long long long long long long long long long long long long long long long long long long long long long long long long long long message.'
hex_message =  [f'0x{x.encode("utf-8").hex().upper()}' for x in original_message]
print(', '.join(hex_message))
