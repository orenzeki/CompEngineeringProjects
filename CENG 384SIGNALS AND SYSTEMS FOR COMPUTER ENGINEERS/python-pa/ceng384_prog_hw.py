#Emrah Kosen e1942317



import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from scipy.io import wavfile

def tic():
    import time
    global startTime_for_tictoc
    startTime_for_tictoc = time.time()

def toc():
    import time
    if 'startTime_for_tictoc' in globals():
        print "Elapsed time is " + str(time.time() - startTime_for_tictoc) + " seconds."
    else:
        print "Toc: start time not set"

class Sound:
    def __init__(self, filename, verbose=False):
        # Load data
        sampling_rate, sound_data = wavfile.read(filename)

        #Convert data to floating values ranging from -1 to 1:
        sound_data = sound_data / (2. ** 15)

        #If two channels, then select only one channel
        if sound_data.ndim > 1:
            sound_data = sound_data[:, 0]

        self.data = sound_data
        self.sampling_rate = sampling_rate
        self.duration = sound_data.shape[0] / float(sampling_rate)
        self.number_of_samples = sound_data.shape[0]

        if verbose:
            print "Sound() is finishing with:"
            print "\t source file: ", filename
            print "\t sampling rate (Hz): ", self.sampling_rate
            print "\t duration (s): ", self.duration
            print "\t number of samples: ", self.number_of_samples


    def plot_sound(self, name=""):
        #Create an array of time values
        time_values = np.arange(0, self.number_of_samples, 1) / float(self.sampling_rate)

        #Convert to milliSeconds
        time_values = time_values * 1000

        fig, ax = plt.subplots()

        plt.plot(time_values, self.data, color='G')
        plt.xlabel('Time (ms)')
        plt.ylabel('Amplitude')
        plt.title(name)
        plt.show()


def dft(x):
    """
        Calculate Discrete Fourier Transform (DFT) for 1D signal x.

        x: A 1D signal of length N.
            Could be a Python iterable object or a Numpy array.

        Output:
            F_x: A 1D complex array, denoting the DFT of x. Its length
                should be N.
    """
    N = len(x)
    F_x = np.zeros(N, dtype=np.complex128)

    ###########################################################################
    #                        YOUR SOLUTION                                    #
    k = 0
    n = 0
    print N
    for k in range(0,N):
        for n in range(0,N):
            F_x[k] += x[n]*np.exp((-2*(1j)*np.pi*n*k)/N)
        #   F_x[k] = x[n]*np.cos( (-2.0*np.pi*n*k)/N) + x[n]*np.sin( (-2.0*np.pi*n*k)/N)

    ###########################################################################

    return F_x


def calculate_spectogram(x, window_size=100, stride=50, dft_function=None):
    """
        Calculate the spectogram (S) for 1D signal x.

        x: A 1D signal of length N.
            Could be a Python iterable object or a Numpy array.
        window_size: The length of the segment for which we will calculate DFT.
        stride: How many samples will be skipped between window positions.
        dft_function: The DFT function to use. Since your implementation
            will be very slow, you should use np.fft.fft() here.

        Output:
            S: A numpy complex 2D array, denoting the S of x.
    """
    N = len(x)

    ###########################################################################
    #                        YOUR SOLUTION                                    #
    ###########################################################################

    ns = N/stride
    na = N%stride
    S = np.zeros((ns,window_size), dtype=np.complex128)
    n = 0
    for n in range(0,ns):
        ran = n*stride
        xt = x[ran:ran+window_size]
	#S[n]=np.fft.fft(xt,window_size)
        S[n] = dft_function(xt,window_size)

    ###########################################################################

    return np.array(S)









def plot_spectogram(S, stride=50, window_size=100, duration=5, Fs=100, name=""):
    t = np.linspace(0, duration, len(S))
    f = np.arange(0, window_size / 2, dtype=np.float) * Fs / window_size # Hz

    # Since DFT is symmetric, we take only half:
    S = S[:, 0:int(window_size/2)]

    # find magnitude of S
    S = np.abs(S) #/ (window_size/2)

    # logarithmic scale (decibel)
    epsilon = 10**-13
    S = 20 * np.log10(S+epsilon) # To get rid of division-by-zero errors

    fig, ax = plt.subplots()
    plt.pcolormesh(t, f, S.transpose(), cmap=plt.get_cmap('jet'))

    #plt.yscale('symlog', linthreshy=1000, linscaley=0.25)
    #ax.yaxis.set_major_formatter(matplotlib.ticker.ScalarFormatter())

    plt.xlim(t[0], t[-1])
    plt.ylim(f[0], f[-1])

    plt.xlabel("Time (s)")
    plt.ylabel("Frequency (Hz)")

    plt.title(name)

    cbar = plt.colorbar()
    cbar.set_label("Intensity (dB)")








def analyze_sound(filename, name):
    signal = Sound(filename)
    signal.plot_sound(name)
    x = signal.data
    Fs = signal.sampling_rate
    duration = signal.duration

    S = calculate_spectogram(x, window_size=window_size, stride=stride, dft_function=np.fft.fft)
    plot_spectogram(S, stride, window_size, duration=duration, Fs=Fs, name=name)

window_size = 500
stride = 100

# Cat sound
cat_sound_filename = 'sound_samples/cat.wav'
analyze_sound(cat_sound_filename, "Cat")

# Dog sound
cow_sound_filename = 'sound_samples/dog.wav'
analyze_sound(cow_sound_filename, "Dog")

# Cow sound
cow_sound_filename = 'sound_samples/cow.wav'
analyze_sound(cow_sound_filename, "Cow")

cow_sound_filename = 'sound_samples/airplane.wav'
analyze_sound(cow_sound_filename, "Airplane")

cow_sound_filename = 'sound_samples/duck.wav'
analyze_sound(cow_sound_filename, "Duck")


cow_sound_filename = 'sound_samples/hello_female.wav'
analyze_sound(cow_sound_filename, "Hello Female")

cow_sound_filename = 'sound_samples/photocopy.wav'
analyze_sound(cow_sound_filename, "Photocopy")



cow_sound_filename = 'sound_samples/photocopy.wav'
analyze_sound(cow_sound_filename, "Photocopy")



# We observe that when a sound is thin, the frequency is higher. For example, the sound of dog is the thinnest sound and the frequency of this sound is the highest.
# When a sound is thich is bold, the frequency is lower. For example , the ound of female is thicher then other sound so frequency is lower than others.
# In airplane sound when times goes on the sound of airplane increase and the same way intensity is increase by the time goes.
# when photocopy sound getting thinner at those time the frequency is become higer. Than the thin part finished the frequency of sound is dropped.
# When the cat mews, the frequency is getting higher.
#
#
