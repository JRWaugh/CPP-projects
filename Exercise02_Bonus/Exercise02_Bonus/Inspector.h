class Inspector {
private:
	float lower_limit, upper_limit;
public:
	bool isInLimits(float number);
	void setLimits(float l_limit, float u_limit);
};

bool Inspector::isInLimits(float number) {
	if (number >= lower_limit && number <= upper_limit)
		return true;
	else
		return false;
}

void Inspector::setLimits(float l_limit, float u_limit) {
	lower_limit = l_limit;
	upper_limit = u_limit;
}