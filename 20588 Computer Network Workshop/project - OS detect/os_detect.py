import argparse
from detect_methods import *


def main():
	"""
	:return: set - Set of optional Operating Systems of the given IP
	"""
	parser = argparse.ArgumentParser("OS detect!")
	parser.add_argument("ip_address", help="The IP address of the OS we wish to detect")
	parser.add_argument("--icmp", help="Test OS using icmp method", action="store_true")
	parser.add_argument("--tcp", help="Test OS using tcp method", action="store_true")
	parser.add_argument("--verbose", help="Print more data", action="store_true")
	args = parser.parse_args()

	result_set = ALL_OS

	if not (args.tcp or args.icmp):
		parser.error("No method of detection supplied. You must supply either --tcp, --icmp or both. ")

	if args.icmp:
		icmp_os_set = test_os_using_icmp(dst_ip=args.ip_address, verbose=args.verbose)
		result_set.intersection_update(icmp_os_set)

	if args.tcp:
		tcp_os_set = test_os_using_tcp(dst_ip=args.ip_address, verbose=args.verbose)
		result_set.intersection_update(tcp_os_set)

	print "The OS is one of the following: {0}".format(result_set)
	return result_set


if __name__ == '__main__':
	main()
