int main(void)
{
	long long l;
	unsigned long long ul;
	signed long long sl;

	return ((sizeof(l) >= 8) && (sizeof(ul) >= 8) && sizeof(sl) >= 8) ? 0 : 1;
}
