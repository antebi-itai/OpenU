from network_utils import *


def test_os_using_icmp(dst_ip, verbose=False):
	"""
	Tests a machine operating system using the ICMP protocol
	The way to do so is to send a PING to the remote host,
	and analyse the properties of the reply.
	:param dst_ip: str - an IPv4 legal ip address - the address of the machine we wish to test
	:param verbose: bool - print relevant information during the process
	:return: set - Set of optional Operating Systems of the given IP
	"""
	result_set = ALL_OS
	if verbose:
		print "[ICMP test] OS options are: {0}".format(result_set)

	icmp_reply = get_icmp_reply(dst_ip)
	if icmp_reply is None:
		print "Destenation host did not respond to icmp request. Was unable to narrow the OS options using icmp. "
		return ALL_OS

	# IP
	ip_layer = icmp_reply.getlayer("IP")
	ip_layer_os_set = os_set_from_ip_layer(ip_layer, verbose=verbose)
	result_set.intersection_update(ip_layer_os_set)
	if verbose:
		print "[ICMP test] OS options are: {0}".format(result_set)

	return result_set


def test_os_using_tcp(dst_ip, verbose=False):
	"""
	Tests a machine operating system using the TCP protocol.
	The way to do so is to find an open TCP port at the remote host,
	and analyse the TCP-IP properties of a connection.
	:param dst_ip: str - an IPv4 legal ip address - the address of the machine we wish to test
	:param bool - verbose: print relevant information during the process
	:return: set - Set of optional Operating Systems of the given IP
	"""
	result_set = ALL_OS
	if verbose:
		print "[TCP test] OS options are: {0}".format(result_set)

	syn_ack = get_syn_ack(dst_ip)
	if syn_ack is None:
		print "Could not find an open TCP port. Was unable to narrow the OS options using tcp. "
		return ALL_OS

	# IP
	ip_layer = syn_ack.getlayer("IP")
	ip_layer_os_set = os_set_from_ip_layer(ip_layer, verbose=verbose)
	result_set.intersection_update(ip_layer_os_set)
	if verbose:
		print "[TCP test] OS options are: {0}".format(result_set)

	# TCP
	tcp_layer = syn_ack.getlayer("TCP")
	tcp_layer_os_set = os_set_from_tcp_layer(tcp_layer, verbose=verbose)
	result_set.intersection_update(tcp_layer_os_set)
	if verbose:
		print "[TCP test] OS options are: {0}".format(result_set)

	return result_set
