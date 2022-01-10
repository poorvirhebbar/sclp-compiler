int a;
void main() {
	int b;
	{
		{
			a = 5;
		}
		b = 6;
	}
	{
		{
			{
				{
					{
						a = a + 1;
					}
				}
			}
		}
	}
}